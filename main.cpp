#include <iostream>
#include <string>
#include "load_map.h"
#include "draw_map.h"

using namespace std;

int main(int argv, char **args) {
  Map *map = NULL;
  LoadMap *loader = new LoadFromXML;
  DrawMap *drawer = new SimpleDraw;

  cout << "Enter file name:" << endl;
  string file_name;
  cin >> file_name;

  map = loader->load(file_name);
  map->print();
  drawer->draw(map, argv, args);

  delete map;
  delete loader;

  return 0;
}