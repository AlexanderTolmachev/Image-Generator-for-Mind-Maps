#include <iostream>
#include "mind_map.h"

//            MapNode              //

MapNode::MapNode() : position_(NONE) {
  coordinates_.x = 0;
  coordinates_.y = 0;
}

MapNode::~MapNode() {
  for (size_t i = 0; i < children_.size(); ++i) {
    delete children_[i];
  }
}

std::string MapNode::text() const {
  return text_;
}

std::vector<const MapNode*> MapNode::children() const {
  std::vector<const MapNode*> children_collection;
  std::vector<MapNode*>::const_iterator children_iterator = children_.begin();
  for ( ; children_iterator != children_.end(); ++children_iterator) {
    children_collection.push_back(*children_iterator);
  }
  return children_collection;
}

NodePosition MapNode::position() const {
  return position_;
}

Point MapNode::coordinates() const {
  return coordinates_;
}

void MapNode::set_position(NodePosition position) {
  position_ = position;
}

void MapNode::set_coordinates(Point coordinates) {
  coordinates_ = coordinates;
}

//           Map            //


Map::Map() : root_(NULL) {}

Map::~Map() {
  delete root_;
}

MapNode *Map::root() const {
  return root_;
}



/*
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
*/

void Map::print() {
  if (root_ == NULL) {
    return;
  }
  print_node(root_, 0);
}

void Map::print_node(MapNode const *node, size_t indent) {
  if (node == NULL) {
    return;
  }
  for (size_t i = 0; i < indent; ++i) {
    std::cout << " ";
  }
  std::cout << node->text() << std::endl;
  std::vector<const MapNode *> children = node->children();
  std::vector<const MapNode *>::const_iterator child = children.begin();
  for ( ; child != children.end(); ++child) {
    print_node(*child, indent + 1);
  }
}