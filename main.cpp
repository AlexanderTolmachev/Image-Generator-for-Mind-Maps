#include <iostream>
#include <string>
#include "map_loader.h"
#include "map_drawer.h"
#include "mind_map.h"
#include "map_settings.h"
 
using std::cout;
using std::cin;
using std::endl;

int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Invalid parametrs number" << endl;
    return 0;
  }
  std::string file_name = argv[1];
  std::string map_settings_file_name = argv[2];
  
  Map *map = NULL;
  MapLoader *map_xml_loader = new XMLLoader();
  MapSettings *map_settings = MapSettings::get_map_settings();
  if (!map_settings->initialize(map_settings_file_name)) {
    cout << "Invalid map settings file" << endl;
    return 0;
  }
  map = map_xml_loader->load(file_name);
  if (map == NULL) {
    cout << "Invalid map file" << endl;    
    return 0;
  }
  int qt_application_argc = 0;
  MapDrawer *map_drawer = new FreeMindDrawer(qt_application_argc, NULL);
  map_drawer->draw(map);
  delete map_xml_loader;
  delete map_drawer;
  delete map;

  return 0;
}