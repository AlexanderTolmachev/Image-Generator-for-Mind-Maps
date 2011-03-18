#pragma once
#include "mind_map.h"
#include "lib\pugixml.hpp"

class MapLoader {
 public:
  virtual Map *load(std::string const &file_name) = 0;

 protected:
  class MutableMapNode : public MapNode {
   public:
    void set_text(std::string text);
    void add_child(MapNode *child);    
  };
  class MutableMap : public Map {
   public:
    void set_root(MapNode *root);
  };
};

class XMLLoader : public MapLoader {
 public:
  virtual Map *load(std::string const &file_name);

 private:
  MapNode *get_dom(pugi::xml_node const &node);
};
