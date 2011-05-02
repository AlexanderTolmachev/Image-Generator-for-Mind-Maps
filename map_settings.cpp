#include "map_settings.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/info_parser.hpp"
#include "boost/property_tree/exceptions.hpp"


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

bool MapSettings::initialize(std::string file_name) {
  using boost::property_tree::ptree;
  using boost::property_tree::ptree_error;
  ptree property_tree;
  try { 
    read_info(file_name, property_tree);
  } catch (ptree_error &) {
    return false;
  }
  std::string output_file_name = property_tree.get("MapSettings.OutputFileName", "error");
  if (output_file_name == "error") {
    return false;
  }
  output_file_name_ = output_file_name;
  std::string output_file_format = property_tree.get("MapSettings.OutputFileFormat", "error");
  if (output_file_format == "error") {
    return false;
  }
  output_file_format_ = output_file_format;
  std::string node_style = property_tree.get("MapSettings.NodeStyle", "error");
  if (node_style == "error") {
    return false;
  }
  node_style_ = node_style;
  std::string edge_style = property_tree.get("MapSettings.EdgeStyle", "error");
  if (edge_style == "error") {
    return false;
  }
  edge_style_ = edge_style;
 
  return true;
}

std::string MapSettings::output_file_name() const {
  return output_file_name_;
}

std::string MapSettings::output_file_format() const {
  return output_file_format_;
}

std::string MapSettings::node_style() const {
  return node_style_;
}

std::string MapSettings::edge_style() const {
  return edge_style_;
}  