#pragma once
#include <map>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include "mind_map.h"

class MapDrawer {
 public:
  virtual void draw(const Map *map) = 0;
};

class SimpleDrawer : public MapDrawer {
 public:
  virtual void draw(const Map *map);
};

class FreeMindDrawer : public MapDrawer {
public:
  FreeMindDrawer(int &argc, char **argv);
  virtual void draw(const Map *map);
private:
  struct Point {
    Point();
    int x;
    int y;
  };
  struct NodeDrawingInfo {
    NodeDrawingInfo();
    Point coordinates;
    int width;
    int height;
    QFont qt_font;
  };

  typedef std::map<const MapNode *, NodeDrawingInfo> NodesDrawingInfoMap;

  void draw_node(const MapNode *node, NodePosition x_offset_direction, QPainter &painter);  
  void lay_out_map(const Map *map);
  void lay_out_node(const MapNode *node, const MapNode *parent, NodePosition x_offset_direction);

  QApplication qt_apprication_;
  NodesDrawingInfoMap node_drawing_info_;  
  Point left_top_map_point;
  Point right_bottom_map_point;
  int x_step_;
  int y_step_;
  int right_subtree_max_y_;
  int left_subtree_max_y_;
  int right_subtree_y_offset_;
  int left_subtree_y_offset_;
};