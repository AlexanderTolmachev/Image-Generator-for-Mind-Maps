#pragma once
#include "mind_map.h"

class MapDrawer {
 public:
  virtual void draw(Map *map) = 0;
};

class SimpleDrawer : public MapDrawer {
 public:
  virtual void draw(Map *map);
};

class FreeMindDrawer : public MapDrawer {
public:
  virtual void draw(Map *map);
private:
  int lay_out(MapNode *node);
  static int x_step;
};