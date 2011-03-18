#include "map_settings.h"


//            MapSettingsDeleter             //

MapSettingsDeleter::MapSettingsDeleter() {}

MapSettingsDeleter::~MapSettingsDeleter() {
  delete MapSettings::map_settings_;
}

//                MapSettings                //

MapSettings *MapSettings::map_settings_ = NULL;
MapSettingsDeleter MapSettings::deleter_;

MapSettings::MapSettings() {}

MapSettings::~MapSettings() {}

MapSettings *MapSettings::get_map_settings() {
  if (map_settings_ == NULL) {
    map_settings_ = new MapSettings;
  }
  return map_settings_;
}

