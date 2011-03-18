#include <string> 
#include "map_loader.h"

using namespace pugi; 

//       MutableMapNode       //

void MapLoader::MutableMapNode::set_text(std::string text) {
  text_ = text;
}

void MapLoader::MutableMapNode::add_child(MapNode *child) {
  if (child != NULL) {
    children_.push_back(child);
  }
}

//        MutableMap          //

void MapLoader::MutableMap::set_root(MapNode *root) {
  root_ = root;
}

//         XMLLodaer          //

Map *XMLLoader::load(const std::string &file_name) {
  xml_document map_xml;
  xml_parse_result parse_result = map_xml.load_file(file_name.c_str());
  if (!parse_result) {
    return NULL;
  }
  xml_node root_xml = map_xml.first_child().first_child();
  MapNode *root = get_dom(root_xml);
  MutableMap *map = new MutableMap();
  map->set_root(root);
  return map; 
}

MapNode *XMLLoader::get_dom(const pugi::xml_node &node) {
  if (!node) {
    return NULL;
  }
  MutableMapNode *map_node = new MutableMapNode();
  xml_attribute attribute_text = node.attribute("TEXT");
  map_node->set_text(attribute_text.value());  
  xml_attribute attribute_position = node.attribute("POSITION");
  if (attribute_position) {
    if (!strcmp(attribute_position.value(), "right")) {
      map_node->set_position(RIGHT);
    } else {
      map_node->set_position(LEFT);
    }
  }

  xml_node child = node.first_child();
  for (; child; child = child.next_sibling())
  {
    if (!strcmp(child.name(),"node"))
    {
      MapNode *map_node_child = get_dom(child);
      map_node->add_child(map_node_child);
    }
  }
  return map_node;
}