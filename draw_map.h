#ifndef DRAW_MAP_
#define DRAW_MAP_

#include "mind_map.h"

class DrawMap {
 public:
  virtual void draw(Map *map, int argv, char **args) = 0;
};

class SimpleDraw : public DrawMap {
 public:
  void draw(Map *map, int argv, char **args);
};

#endif