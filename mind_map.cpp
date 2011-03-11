#include "mind_map.h"
#include <iostream>

//            MapNode              //

MapNode::MapNode() {}

MapNode::~MapNode() {
  for (size_t i = 0; i < children_.size(); ++i) {
    delete children_[i];
  }
}

std::string MapNode::text() const {
  return text_;
}

MapNode *MapNode::child(size_t i) const {
  if (i > children_.size()) {
    return NULL;
  }
  return children_[i];
}

size_t MapNode::children_number() const {
  return children_.size();
}

void MapNode::set_text(std::string text) {
  text_ = text;
}

void MapNode::add_child(MapNode *child) {
  if (child != NULL) {
    children_.push_back(child);
  }
}

//           Map            //

Map::Map() : map_(NULL) {}

Map::Map(MapNode *map) : map_(map) {}

Map::~Map() {
  delete map_;
}

MapNode *Map::map() const {
  return map_;
}

void Map::set_map(MapNode *map) {
  map_ = map;
}

void PrintNode(MapNode *node, size_t space_number) {
  if (node == NULL) {
    return;
  }
  for (size_t i = 0; i < space_number; ++i) {
    std::cout << " ";
  }
  std::cout << node->text() << std::endl;
  for (size_t i = 0; i < node->children_number(); ++i) {
    PrintNode(node->child(i), space_number + 1);
  }
}

void Map::print() {
  PrintNode(map_, 0);
}