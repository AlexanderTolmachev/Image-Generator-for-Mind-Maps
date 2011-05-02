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
  bool initialize(std::string file_name);
  std::string output_file_name() const;
  std::string output_file_format() const;
  std::string node_style() const;
  std::string edge_style() const;
 private:
  static MapSettings *map_settings_;
  static MapSettingsDeleter deleter_;
  std::string output_file_name_;
  std::string output_file_format_;
  std::string node_style_;
  std::string edge_style_;
  friend class MapSettingsDeleter;
};
