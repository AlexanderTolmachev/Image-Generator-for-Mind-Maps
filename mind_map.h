#pragma once
#include <string>
#include <vector>

enum NodePosition {
  NONE,
  RIGHT,
  LEFT,
  ROOT
};

struct Point {
  Point() : x(0), y(0) {}
  int x;
  int y;
};

struct NodeTextFont {
  NodeTextFont() 
      : name("SansSerif"), 
        size(12), 
        is_bold(false), 
        is_italic(false) {}
  std::string name;
  size_t size;
  bool is_bold;
  bool is_italic;
};

struct ArrowLink {
  ArrowLink() : start_arrow(false), end_arrow(false) {}
  std::string destination_node_id;
  bool start_arrow;
  bool end_arrow;
  Point start_inclination;
  Point end_inclination;
  std::string color;
};

class MapNode {
 public: 
  MapNode();
  ~MapNode();

  std::string text() const;
  std::string id() const;
  NodePosition position() const;
  NodeTextFont font() const;
  std::string text_color() const;
  std::string background_color() const;
  std::vector<std::string> icons() const;
  std::vector<ArrowLink> arrow_links() const;
  const MapNode *parent() const;
  std::vector<const MapNode *> children() const; 
 protected:
  std::string text_;
  std::string id_;
  NodePosition position_;
  NodeTextFont font_;
  std::string text_color_;
  std::string background_color_;
  std::vector<std::string> icons_;
  std::vector<ArrowLink> arrow_links_;
  MapNode *parent_;
  std::vector<MapNode *> children_;
};

class Map {
 protected:
  Map();
 public:
  ~Map();
  const MapNode *root() const;
  const MapNode *find_node_by_id(const std::string &node_id) const;
  void print();
 protected:
  void print_node(const MapNode *node, size_t indent) const ;
  const MapNode *find_node(const MapNode *node, const std::string &node_id) const;

  MapNode *root_;
};