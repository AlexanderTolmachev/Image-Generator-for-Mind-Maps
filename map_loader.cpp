#include <string>
#include <cstring>
#include <cstdlib>
#include "map_loader.h"

using namespace pugi; 

//       MutableMapNode       //

void MapLoader::MutableMapNode::set_text(std::string text) {
  text_ = text;
}

void MapLoader::MutableMapNode::set_id(std::string id) {
  id_ = id;
}

void MapLoader::MutableMapNode::set_position(NodePosition position) {
  position_ = position;
}

void MapLoader::MutableMapNode::set_font(NodeTextFont font) {
  font_ = font;
}

void MapLoader::MutableMapNode::set_text_color(std::string text_color) {
  text_color_ = text_color;
}

void MapLoader::MutableMapNode::set_background_color(std::string background_color) {
  background_color_ = background_color;
}

void MapLoader::MutableMapNode::add_icon(std::string icon_name) {
  icons_.push_back(icon_name);
}

void MapLoader::MutableMapNode::add_arrow_link(ArrowLink arrow_link) {
  arrow_links_.push_back(arrow_link);
}

void MapLoader::MutableMapNode::set_parent(MapNode *parent) {
  parent_ = parent;
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
  MapNode *root = parse_node(root_xml, NULL);
  MutableMap *map = new MutableMap();
  map->set_root(root);
  return map; 
}

MapNode *XMLLoader::parse_node(const pugi::xml_node &node, MapNode *parent) {
  if (!node) {
    return NULL;
  }
  MutableMapNode *map_node = new MutableMapNode();
  xml_attribute attribute_text = node.attribute("TEXT");
  map_node->set_text(attribute_text.value());  
  xml_attribute attribute_id = node.attribute("ID");
  map_node->set_id(attribute_id.value());
  xml_attribute attribute_position = node.attribute("POSITION");
  if (attribute_position) {
    if (!strcmp(attribute_position.value(), "right")) {
      map_node->set_position(RIGHT);
    } else {
      map_node->set_position(LEFT);
    }
  } else if (parent == NULL) {
    map_node->set_position(ROOT);
  }
  xml_attribute attribute_text_color = node.attribute("COLOR");
  if (attribute_text_color) {
    std::string color = attribute_text_color.value();
    map_node->set_text_color(color);
  }
  xml_attribute attribute_background_color = node.attribute("BACKGROUND_COLOR");
  if (attribute_background_color) {
    std::string color = attribute_background_color.value();
    map_node->set_background_color(color);
  }
  xml_node font = node.child("font");
  if (font) {
    NodeTextFont map_node_font;
    xml_attribute font_name = font.attribute("NAME");
    xml_attribute font_size = font.attribute("SIZE");
    xml_attribute font_bold = font.attribute("BOLD");
    xml_attribute font_italic = font.attribute("ITALIC");
    if (font_name) {
      map_node_font.name = font_name.value();
    }
    if (font_size) {
      map_node_font.size = font_size.as_uint();
    }
    if (font_bold) {
      map_node_font.is_bold = font_bold.as_bool();
    }
    if (font_italic) {
      map_node_font.is_italic = font_italic.as_bool();
    }
    map_node->set_font(map_node_font);
  }

  map_node->set_parent(parent);
  xml_node child = node.first_child();
  for (; child; child = child.next_sibling()) {
    if (!strcmp(child.name(), "icon")) {
      xml_attribute icon_name = child.attribute("BUILTIN");
      map_node->add_icon(icon_name.value());
    } else if (!strcmp(child.name(),"arrowlink")) {
      xml_attribute attribute_destination = child.attribute("DESTINATION");
      xml_attribute attribute_start_arrow = child.attribute("STARTARROW");
      xml_attribute attribute_end_arrow = child.attribute("ENDARROW");
      xml_attribute attribute_start_inclination = child.attribute("STARTINCLINATION");
      xml_attribute attribute_end_inclination = child.attribute("ENDINCLINATION");
      xml_attribute attibute_color = child.attribute("COLOR");
      ArrowLink arrow_link;
      arrow_link.destination_node_id = attribute_destination.value();
      if (!strcmp(attribute_start_arrow.value(), "None")) {
        arrow_link.start_arrow = false;
      } else {
        arrow_link.start_arrow = true;
      }
      if (!strcmp(attribute_end_arrow.value(), "None")) {
        arrow_link.end_arrow = false;
      } else {
        arrow_link.end_arrow = true;
      }
      std::string start_inclination = attribute_start_inclination.value();
      arrow_link.start_inclination = parse_string_to_point(start_inclination, ';');
      std::string end_inclination = attribute_end_inclination.value();
      arrow_link.end_inclination = parse_string_to_point(end_inclination, ';');
      if (attibute_color) {
        arrow_link.color = attibute_color.value();
      }
      map_node->add_arrow_link(arrow_link);
    }  else if (!strcmp(child.name(),"node")) {
      MapNode *map_node_child = parse_node(child, map_node);
      map_node->add_child(map_node_child);
    }
  }
  return map_node;
}

Point XMLLoader::parse_string_to_point(const std::string &string, const char delimiter) {
  std::string input_string = string;
  Point result;
  size_t delimiter_pos = input_string.find(delimiter);
  std::string buf;
  buf.assign(input_string, 0, delimiter_pos);
  result.x = atoi(buf.c_str());
  input_string.erase(0, delimiter_pos + 1);
  delimiter_pos = input_string.find(delimiter);
  buf.assign(input_string, 0, delimiter_pos);
  result.y = atoi(buf.c_str());
  return result;
}