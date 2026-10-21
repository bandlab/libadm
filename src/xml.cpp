#include "adm/xml.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "adm/errors.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace adm::xml {
  namespace {
    bool whitespace(char c) {
      return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    using Node = rapidxml::xml_node<>;
    Node* requireNode(Node* node) {
      if (!node) throw std::logic_error("empty XML node");
      return node;
    }

    char* copyString(Node* node, std::string_view text) {
      auto* result =
          node->document()->allocate_string(nullptr, text.size() + 1);
      if (!text.empty()) std::copy(text.begin(), text.end(), result);
      result[text.size()] = '\0';
      return result;
    }
  }  // namespace

  void validateXml(std::string_view text) {
    if (text.find('\0') != std::string_view::npos)
      throw error::XmlParsingError("embedded NUL in XML");
    std::vector<std::string_view> elements;
    std::optional<std::size_t> customStart;
    std::size_t customDepth = 0;
    auto fail = [](const char* message) {
      throw error::XmlParsingError(message);
    };
    auto checkSize = [&](std::size_t end) {
      if (customStart && end - *customStart > maxCustomElementBytes)
        fail("audioFormatCustom exceeds 4 MiB");
    };
    std::size_t offset = 0;
    while (offset < text.size()) {
      const auto start = text.find('<', offset);
      if (start == std::string_view::npos) {
        checkSize(text.size());
        break;
      }
      checkSize(start);
      const auto tail = text.substr(start);
      std::string_view terminator;
      if (tail.starts_with("<!--"))
        terminator = "-->";
      else if (tail.starts_with("<![CDATA["))
        terminator = "]]>";
      else if (tail.starts_with("<?"))
        terminator = "?>";
      if (!terminator.empty()) {
        const auto end =
            text.find(terminator, start + (terminator == "?>" ? 2 : 4));
        if (end == std::string_view::npos) fail("unterminated XML construct");
        offset = end + terminator.size();
        checkSize(offset);
        continue;
      }
      if (tail.starts_with("<!DOCTYPE")) {
        std::size_t end = start + 9;
        std::size_t brackets = 0;
        char quote = 0;
        for (; end < text.size(); ++end) {
          const auto c = text[end];
          if (quote) {
            if (c == quote) quote = 0;
          } else if (text.substr(end).starts_with("<!--")) {
            const auto commentEnd = text.find("-->", end + 4);
            if (commentEnd == std::string_view::npos)
              fail("unterminated DTD comment");
            end = commentEnd + 2;
          } else if (c == '\'' || c == '"')
            quote = c;
          else if (c == '[')
            ++brackets;
          else if (c == ']') {
            if (!brackets) fail("invalid DTD");
            --brackets;
          } else if (c == '>' && brackets == 0)
            break;
        }
        if (end == text.size()) fail("unterminated DTD");
        offset = end + 1;
        checkSize(offset);
        continue;
      }
      if (tail.starts_with("<!")) {
        fail("unsupported XML declaration");
      }
      std::size_t end = start + 1;
      char quote = 0;
      for (; end < text.size(); ++end) {
        const auto c = text[end];
        if (quote) {
          if (c == quote) quote = 0;
        } else if (c == '\'' || c == '"')
          quote = c;
        else if (c == '>')
          break;
      }
      if (end == text.size()) fail("unterminated XML tag");
      const bool closing = text[start + 1] == '/';
      const auto nameStart = start + (closing ? 2 : 1);
      auto nameEnd = nameStart;
      while (nameEnd < end && !whitespace(text[nameEnd]) &&
             text[nameEnd] != '/')
        ++nameEnd;
      const auto name = text.substr(nameStart, nameEnd - nameStart);
      if (name.empty()) fail("empty XML tag name");
      if (closing) {
        if (elements.empty() || elements.back() != name)
          fail("mismatched XML closing tag");
        for (auto i = nameEnd; i < end; ++i)
          if (!whitespace(text[i])) fail("invalid XML closing tag");
        elements.pop_back();
      } else {
        if (elements.size() >= maxXmlDepth)
          fail("XML nesting exceeds 128 levels");
        if (name == "audioFormatCustom" && !customStart) {
          customStart = start;
          customDepth = elements.size();
        }
        if (text[end - 1] != '/') elements.push_back(name);
      }
      offset = end + 1;
      checkSize(offset);
      if (customStart && elements.size() == customDepth) customStart.reset();
    }
    if (!elements.empty()) fail("unclosed XML element");
  }

  struct DomDocument::Impl {
    std::string buffer;
    rapidxml::xml_document<> document;
  };

  DomDocument::DomDocument() : impl_(std::make_unique<Impl>()) {}
  DomDocument::~DomDocument() = default;

  void DomDocument::parse(std::string_view text) {
    validateXml(text);
    auto parsed = std::make_unique<Impl>();
    parsed->buffer.assign(text);
    parsed->document.parse<
        rapidxml::parse_validate_closing_tags | rapidxml::parse_comment_nodes |
        rapidxml::parse_declaration_node | rapidxml::parse_doctype_node |
        rapidxml::parse_pi_nodes>(parsed->buffer.data());
    if (!parsed->document.first_node())
      throw error::XmlParsingError("xml document is empty");
    impl_ = std::move(parsed);
  }

  void DomDocument::parse(std::istream& stream) {
    const std::string text{std::istreambuf_iterator<char>(stream), {}};
    if (stream.fail() || stream.bad())
      throw std::runtime_error("failed to read XML stream");
    parse(text);
  }

  DomNode DomDocument::root() { return DomNode(&impl_->document); }

  std::string DomDocument::toString() const {
    std::ostringstream stream;
    stream << impl_->document;
    return stream.str();
  }

  std::string_view DomNode::name() const {
    const auto* node = requireNode(node_);
    return {node->name(), node->name_size()};
  }
  std::string_view DomNode::value() const {
    const auto* node = requireNode(node_);
    return {node->value(), node->value_size()};
  }
  std::optional<std::string_view> DomNode::attribute(
      std::string_view name) const {
    if (name.empty()) throw std::invalid_argument("empty XML attribute name");
    const auto* attr =
        requireNode(node_)->first_attribute(name.data(), name.size());
    if (!attr) return std::nullopt;
    return std::string_view(attr->value(), attr->value_size());
  }
  DomNode DomNode::firstNode(std::string_view name) const {
    return DomNode(requireNode(node_)->first_node(
        name.empty() ? nullptr : name.data(), name.size()));
  }
  DomNode DomNode::nextSibling(std::string_view name) const {
    return DomNode(requireNode(node_)->next_sibling(
        name.empty() ? nullptr : name.data(), name.size()));
  }
  DomNode DomNode::findNode(std::string_view name) const {
    auto* root = requireNode(node_);
    auto* node = root->first_node();
    while (node) {
      if (node->type() == rapidxml::node_element &&
          std::string_view(node->name(), node->name_size()) == name)
        return DomNode(node);
      if (node->first_node())
        node = node->first_node();
      else {
        while (node != root && !node->next_sibling()) node = node->parent();
        if (node == root) break;
        node = node->next_sibling();
      }
    }
    return {};
  }
  DomNode DomNode::addNode(std::string_view name) {
    if (name.empty()) throw std::invalid_argument("empty XML element name");
    auto* parent = requireNode(node_);
    auto* child = parent->document()->allocate_node(
        rapidxml::node_element, copyString(parent, name), nullptr, name.size());
    parent->append_node(child);
    return DomNode(child);
  }
  void DomNode::setValue(std::string_view value) {
    auto* node = requireNode(node_);
    node->remove_all_nodes();
    node->value(copyString(node, value), value.size());
  }
  void DomNode::setAttribute(std::string_view name, std::string_view value) {
    if (name.empty()) throw std::invalid_argument("empty XML attribute name");
    auto* node = requireNode(node_);
    if (auto* attr = node->first_attribute(name.data(), name.size())) {
      attr->value(copyString(node, value), value.size());
    } else {
      node->append_attribute(node->document()->allocate_attribute(
          copyString(node, name), copyString(node, value), name.size(),
          value.size()));
    }
  }
}  // namespace adm::xml
