#pragma once
#include <map>
#include "QtGui/QApplication"
#include "QtGui/QPainter"
#include "QtGui/QFont"

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
  struct NodeDrawingInfo {
    NodeDrawingInfo();
    Point coordinates;
    int width;
    int height;
    QFont qt_font;
  };
  enum EdgeBinding {
    BOTTOM,
    CENTER
  };

  typedef std::map<const MapNode *, NodeDrawingInfo> NodesDrawingInfoMap;

  void draw_node(const Map *map, const MapNode *node, QPainter &painter);  
  void draw_arrow_links(const Map *map, const MapNode *node, QPainter &painter);
  void draw_straight_edge(const Map *map, const MapNode *parent, 
                          const MapNode *child, EdgeBinding edge_binding, 
                          QPainter &painter);
  void draw_curved_edge(const Map *map, const MapNode *parent, 
                        const MapNode *child,  EdgeBinding edge_binding,
                        QPainter &painter);
  void draw_straight_arrow_link(const Map *map, const MapNode *start_node, 
                       const MapNode *end_node, ArrowLink arrow_link, 
                       QPainter &painter);
  void draw_curved_arrow_link(const Map *map, const MapNode *start_node, 
                       const MapNode *end_node, ArrowLink arrow_link, 
                       QPainter &painter);
  void lay_out_map(const Map *map);
  void lay_out_node(const MapNode *node, const MapNode *parent, NodePosition x_offset_direction);
  void update_map_drawing_info(const Map *map);
  void update_root_coordinates(const MapNode *root);
  void update_nodes_coordinates(const MapNode *node, NodePosition x_offset_direction);

  QApplication qt_apprication_;
  NodesDrawingInfoMap node_drawing_info_;  
  int map_width_;
  int map_height_;
  Point left_top_map_point_;
  Point right_bottom_map_point_;
  int x_step_;
  int y_step_;
  int right_subtree_max_y_;
  int left_subtree_max_y_;
  int right_subtree_y_offset_;
  int left_subtree_y_offset_;
  int border_indent_;
  int node_text_indent_;
  int curved_edge_control_point_indent_;
  int straight_arrow_link_border_indent_;
  int straight_arrow_link_dividing_indent_;
  int arrowhead_height_;
  int arrowhead_widht_;
  int bubble_node_rounded_rect_radius_;
  int icon_size_;
};