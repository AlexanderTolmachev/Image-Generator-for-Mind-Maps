#ifndef LOAD_MAP_
#define LOAD_MAP_

#include "mind_map.h"

class LoadMap {
 public:
  virtual Map* load(std::string const &file_name) = 0;
};

class LoadFromXML : public LoadMap {
 public:
  Map* load(std::string const &file_name);
};

#endif