#pragma once
#include <string>
#include <vector>

enum NodePosition {
  NONE,
  RIGHT,
  LEFT,
};

class MapNode {
 public: 
  MapNode();
  ~MapNode();

  std::string text() const;
  std::vector<const MapNode *> children() const;  
  NodePosition position() const;

  void set_position(NodePosition position);
 protected:
  std::string text_;
  NodePosition position_;
  std::vector<MapNode *> children_;
};

class Map {
 protected:
  Map();
 public:
  ~Map();
  const MapNode *root() const;
  void print();

 protected:
  void print_node(MapNode const *node, size_t indent);

  MapNode *root_;
};