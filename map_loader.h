#pragma once
#include "mind_map.h"
#include "lib/pugixml.hpp"

class MapLoader {
 public:
  virtual Map *load(const std::string &file_name) = 0;
 protected:
  class MutableMapNode : public MapNode {
   public:
    void set_text(std::string text);
    void set_id(std::string id);
    void set_position(NodePosition position);
    void set_font(NodeTextFont font);
    void set_text_color(std::string text_color);
    void set_background_color(std::string background_color);
    void add_icon(std::string icon_name);
    void add_arrow_link(ArrowLink arrow_link);
    void set_parent(MapNode *parent);
    void add_child(MapNode *child);    
  };
  class MutableMap : public Map {
   public:
    void set_root(MapNode *root);
  };
};

class XMLLoader : public MapLoader {
 public:
  virtual Map *load(const std::string &file_name);
 private:
  MapNode *parse_node(const pugi::xml_node &node, MapNode *parent);
  Point parse_string_to_point(const std::string &string,  const char delimiter);
};
