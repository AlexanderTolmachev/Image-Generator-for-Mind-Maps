#include <iostream>
#include "mind_map.h"

//            MapNode              //

MapNode::MapNode() : position_(NONE) {}

MapNode::~MapNode() {
  for (size_t i = 0; i < children_.size(); ++i) {
    delete children_[i];
  }
}

std::string MapNode::text() const {
  return text_;
}

std::string MapNode::id() const {
  return id_;
}

NodePosition MapNode::position() const {
  if (position_ == NONE) {
    return parent_->position(); 
  }
  return position_;
}

NodeTextFont MapNode::font() const {
  return font_;
}

std::string MapNode::text_color() const {
  return text_color_;
}

std::string MapNode::background_color() const {
  return background_color_;
}

std::vector<std::string> MapNode::icons() const {
  return icons_;
}

std::vector<ArrowLink> MapNode::arrow_links() const {
  return arrow_links_;
}

const MapNode *MapNode::parent() const {
  return parent_;
}

std::vector<const MapNode*> MapNode::children() const {
  std::vector<const MapNode*> children_collection;
  std::vector<MapNode*>::const_iterator children_iterator = children_.begin();
  for ( ; children_iterator != children_.end(); ++children_iterator) {
    children_collection.push_back(*children_iterator);
  }
  return children_collection;
}

//           Map            //

Map::Map() : root_(NULL) {}

Map::~Map() {
  delete root_;
}

const MapNode *Map::root() const {
  return root_;
}

const MapNode *Map::find_node_by_id(const std::string &node_id) const {
  return find_node(root_, node_id);
}

void Map::print() {
  if (root_ == NULL) {
    return;
  }
  print_node(root_, 0);
}

void Map::print_node(MapNode const *node, size_t indent) const {
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

const MapNode *Map::find_node(const MapNode *node, const std::string &node_id) const {
  if (node->id() == node_id) {
    return node;
  }
  std::vector<const MapNode *> children = node->children();
  if (children.empty()) {
    return NULL;
  }
  std::vector<const MapNode *>::const_iterator child = children.begin();
  for ( ; child != children.end(); ++child) {
    const MapNode *node = find_node(*child, node_id); 
    if (node != NULL) {
      return node;
    }
  }
  return NULL;
}