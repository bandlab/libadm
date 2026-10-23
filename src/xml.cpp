#include "adm/xml.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include "adm/errors.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace adm::xml {
  namespace {
    using Node = rapidxml::xml_node<>;
    Node* requireNode(Node* node) {
      if (!node) throw std::logic_error("empty XML node");
      return node;
    }

    template <typename T>
    std::string_view localNameOf(const T* item) {
      const std::string_view name(item->name(), item->name_size());
      return name.substr(name.rfind(':') + 1);  // npos + 1 == 0
    }

    Node* findElement(Node* node, std::string_view localName) {
      if (localName.empty()) throw std::invalid_argument("empty XML element name");
      for (; node; node = node->next_sibling())
        if (node->type() == rapidxml::node_element &&
            localNameOf(node) == localName)
          return node;
      return nullptr;
    }

    char* copyString(Node* node, std::string_view text) {
      auto* result =
          node->document()->allocate_string(nullptr, text.size() + 1);
      if (!text.empty()) std::copy(text.begin(), text.end(), result);
      result[text.size()] = '\0';
      return result;
    }
  }  // namespace

  struct DomDocument::Impl {
    std::string buffer;
    rapidxml::xml_document<> document;
  };

  DomDocument::DomDocument() : impl_(std::make_unique<Impl>()) {}
  DomDocument::~DomDocument() = default;

  void DomDocument::parse(std::string_view text) {
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
  DomNode DomNode::childElement(std::string_view localName) const {
    return DomNode(findElement(requireNode(node_)->first_node(), localName));
  }
  DomNode DomNode::nextSiblingElement(std::string_view localName) const {
    return DomNode(findElement(requireNode(node_)->next_sibling(), localName));
  }
  std::optional<std::string_view> DomNode::attributeValue(
      std::string_view localName) const {
    if (localName.empty()) throw std::invalid_argument("empty XML attribute name");
    for (auto* attr = requireNode(node_)->first_attribute(); attr;
         attr = attr->next_attribute())
      if (localNameOf(attr) == localName)
        return std::string_view(attr->value(), attr->value_size());
    return std::nullopt;
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
