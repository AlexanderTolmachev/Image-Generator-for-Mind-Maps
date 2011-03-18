#pragma once
#include <string>
#include <vector>

enum NodePosition {
  NONE,
  RIGHT,
  LEFT,
};

struct Point {
  int x;
  int y;
};

class MapNode {
 public: 
  MapNode();
  ~MapNode();

  std::string text() const;
  std::vector<const MapNode*> children() const;  
  NodePosition position() const;
  Point coordinates() const;

  void set_position(NodePosition position);
  void set_coordinates(Point coordinates);

 protected:
  std::string text_;
  NodePosition position_;
  Point coordinates_;
  std::vector<MapNode*> children_;
};

class Map {
 protected:
  Map();
 public:
  ~Map();
  MapNode *root() const;
  void print();

 protected:
  void print_node(MapNode const *node, size_t indent);

  MapNode *root_;
};