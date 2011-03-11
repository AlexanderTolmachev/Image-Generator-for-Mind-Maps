#include <string.h>
#include "pugixml.hpp"
#include "load_map.h"

using namespace pugi;

MapNode* GetDOM(xml_node const &node) {
  if (!node) {
    return NULL;
  }

  MapNode *map_node = new MapNode;
  xml_attribute attribute_text = node.attribute("TEXT");
  map_node->set_text(attribute_text.value());  

  for (xml_node child = node.first_child(); child; child = child.next_sibling())
  {
    if (!strcmp(child.name(),"node"))
    {
      MapNode *map_node_child = GetDOM(child);
      map_node->add_child(map_node_child);
    }
  }
  return map_node;
}

Map *LoadFromXML::load(std::string const &file_name) {
  xml_document map_xml;
  xml_parse_result parse_result = map_xml.load_file(file_name.c_str());
  if (!parse_result) {
    return NULL;
  }
  Map *map = new Map(GetDOM(map_xml.first_child().first_child()));
  return map;
}