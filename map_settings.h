#pragma once
#include <string>

class MapSettingsDeleter {
 public:
  MapSettingsDeleter();
  ~MapSettingsDeleter();
};

class MapSettings {
 private:
  MapSettings();
  MapSettings(MapSettings const &settings);
  ~MapSettings();
  MapSettings &operator = (MapSettings const &settings);

 public:
  static MapSettings *get_map_settings();
  std::string file_format();

 private:
  static MapSettings *map_settings_;
  static MapSettingsDeleter deleter_;
  friend class MapSettingsDeleter;
};
