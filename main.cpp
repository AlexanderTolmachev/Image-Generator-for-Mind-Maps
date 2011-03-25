#include <iostream>
#include <string>
#include "map_loader.h"
#include "map_drawer.h"
#include "mind_map.h"
 
using namespace std;

int main() {
  Map *map = NULL;
  MapLoader *map_xml_loader = new XMLLoader();

  cout << "Enter file name:" << endl;
  string file_name;
  cin >> file_name;

  map = map_xml_loader->load(file_name);
  if (map == NULL) {
    return 0;
  }
  map->print();

  int argc = 0;
  MapDrawer *map_drawer = new FreeMindDrawer(argc, NULL);

  map_drawer->draw(map);

  delete map_drawer;
  delete map;

  return 0;
}