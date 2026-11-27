#pragma once

#include "rapidxml/rapidxml.hpp"
#include <algorithm>

namespace adm {
  namespace xml {
    template <typename It>
    int countLines(It begin, It end) {
      return static_cast<int>(std::count(begin, end, '\n'));
    }

    /// Lines are counted from the name of the document's first element.
    /// Declaration and comment nodes have static empty names outside the
    /// input buffer, so they cannot be the origin.
    template <typename Item>
    char* getLineOrigin(Item* item) {
      auto* node = item->document()->first_node();
      while (node && node->type() != rapidxml::node_element)
        node = node->next_sibling();
      return node ? node->name() : item->name();
    }

    inline int getDocumentLine(rapidxml::xml_node<>* node) {
      return countLines(getLineOrigin(node), node->name());
    }
    inline int getDocumentLine(rapidxml::xml_attribute<>* attr) {
      return countLines(getLineOrigin(attr), attr->name());
    }
  }  // namespace xml
}  // namespace adm
