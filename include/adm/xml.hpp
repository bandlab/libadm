#pragma once

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include "adm/export.h"

namespace rapidxml {
  template <typename Ch>
  class xml_node;
}

namespace adm::xml {
  class DomDocument;
  class DocumentParser;

  /// Non-owning handle; the document must outlive its nodes and string views.
  class ADM_EXPORT DomNode {
   public:
    DomNode() = default;
    explicit operator bool() const { return node_ != nullptr; }
    std::string_view name() const;
    std::string_view value() const;
    std::optional<std::string_view> attribute(std::string_view name) const;
    DomNode firstNode(std::string_view name = {}) const;
    /// Match element/attribute local names without resolving namespace URIs.
    DomNode childElement(std::string_view localName) const;
    DomNode nextSiblingElement(std::string_view localName) const;
    std::optional<std::string_view> attributeValue(
        std::string_view localName) const;
    DomNode nextSibling(std::string_view name = {}) const;
    DomNode findNode(std::string_view name) const;
    DomNode addNode(std::string_view name);
    void setValue(std::string_view value);
    void setAttribute(std::string_view name, std::string_view value);

   private:
    friend class DomDocument;
    friend class DocumentParser;
    explicit DomNode(rapidxml::xml_node<char>* node) : node_(node) {}
    rapidxml::xml_node<char>* node_ = nullptr;
  };

  /// Owning generic XML DOM, independent of the ADM object model.
  class ADM_EXPORT DomDocument {
   public:
    DomDocument();
    ~DomDocument();
    DomDocument(const DomDocument&) = delete;
    DomDocument& operator=(const DomDocument&) = delete;
    void parse(std::string_view text);
    void parse(std::istream& stream);
    DomNode root();
    std::string toString() const;

   private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
  };
}  // namespace adm::xml
