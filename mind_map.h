#ifndef MIND_MAP_H_
#define MIND_MAP_H_

#include <string>
#include <vector>

class MapNode {
 public: 
  MapNode();
  ~MapNode();
  std::string text() const;
  MapNode *child(size_t i) const;
  size_t children_number() const;
  void set_text(std::string text);
  void add_child(MapNode* child);
 private:
  std::string text_;
  std::vector<MapNode*> children_;
};

class Map {
 public:
  Map();
  Map(MapNode *map);
  ~Map();
  MapNode *map() const;
  void set_map(MapNode *map);
  void print();
 private:
  MapNode *map_;
};

//MapNode* LoadFromXMLFile(char const *file_name);
//void PrintMap(MapNode *node);

#endif