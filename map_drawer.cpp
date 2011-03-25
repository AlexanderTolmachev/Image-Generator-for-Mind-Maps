#include <QtGui/QImage>
#include <QtGui/QBrush>

#include "map_drawer.h"

void SimpleDrawer::draw(const Map *map) {
  int argv = 0;
  QApplication app(argv, NULL);
  QImage *Image = new QImage(200, 200, QImage::Format_RGB444);
  QBrush baseBrush(Qt::white, Qt::SolidPattern);
  QPainter painter(Image);
 
  painter.fillRect(0, 0, 200, 200, baseBrush);
  painter.setBackground(baseBrush);
  painter.setBrush(Qt::white);
  painter.setPen(Qt::black);

  QString text = map->root()->text().c_str();
  QFont font("Arial", 25);
  QFontMetrics font_metrics(font);
  int font_width = font_metrics.averageCharWidth();
  int font_height = font_metrics.height();
  int x = 100 - font_width * text.size() / 2;
  int y = 100 - font_height / 2;
  QRect bound_rect = font_metrics.boundingRect(x, y, font_width * text.size(), 20, Qt::AlignCenter, text);

  painter.drawEllipse(bound_rect);
  painter.drawText(bound_rect, Qt::AlignCenter, text);   
  painter.end();

  Image->save("example.png", "PNG");

  delete Image; 
}

FreeMindDrawer::FreeMindDrawer(int &argc, char **argv) 
    : x_step_(30),
      y_step_(20), 
      right_subtree_max_y_ (0),
      left_subtree_max_y_ (0),
      right_subtree_y_offset_ (0),
      left_subtree_y_offset_ (0),
      qt_apprication_(argc, argv) {}

FreeMindDrawer::Point::Point() : x(0), y(0) {}

FreeMindDrawer::NodeDrawingInfo::NodeDrawingInfo() 
    : width(0),
      height(0) {}

void FreeMindDrawer::draw(const Map *map) {
  lay_out_map(map);
  int map_width = right_bottom_map_point.x - left_top_map_point.x;
  int map_height = right_bottom_map_point.y;
  QImage *map_image = new QImage(map_width, map_height, QImage::Format_RGB444);
  QPainter map_painter(map_image);
  QBrush map_background_brush(Qt::white, Qt::SolidPattern);
  map_painter.fillRect(0, 0, map_width, map_height, map_background_brush);
  map_painter.setBackground(map_background_brush);
  map_painter.setBrush(Qt::white);
  map_painter.setPen(Qt::black);

  const MapNode *map_root = map->root();
  std::vector<const MapNode *> root_children = map_root->children();
  std::vector<const MapNode *>::const_iterator child = root_children.begin();
  for ( ; child != root_children.end(); ++child) {
    NodePosition x_offset_direction = (*child)->position();
    draw_node(*child, x_offset_direction, map_painter);
  }
  int x = node_drawing_info_[map_root].coordinates.x - left_top_map_point.x;
  int y = node_drawing_info_[map_root].coordinates.y;
  int root_width = node_drawing_info_[map_root].width;
  int root_height = node_drawing_info_[map_root].height;
  QFont qt_root_font = node_drawing_info_[map_root].qt_font;
  QString root_text = map_root->text().c_str();
  map_painter.setFont(qt_root_font);
  map_painter.drawEllipse(x - root_height/2, y - root_height - root_height/2, 
                          root_width + root_height, root_height + root_height * 1.5);
  map_painter.drawText(x, y, root_text);
  map_painter.end();
  map_image->save("map.png", "PNG");
  delete map_image;
}

void FreeMindDrawer::draw_node(const MapNode *node, 
                               NodePosition x_offset_direction, 
                               QPainter &painter) {
  int x = node_drawing_info_[node].coordinates.x - left_top_map_point.x;
  int y = node_drawing_info_[node].coordinates.y;
  if (x_offset_direction == RIGHT) {
    y += right_subtree_y_offset_;
  } else {
    y += left_subtree_y_offset_;
  }
  int node_width = node_drawing_info_[node].width;
  int node_height = node_drawing_info_[node].height;
  QString node_text = node->text().c_str();
  QFont node_font = node_drawing_info_[node].qt_font;
  painter.setFont(node_font);
  painter.drawRect(x, y - node_height, node_width, node_height);
  painter.drawText(x, y, node_text);
  
  std::vector<const MapNode *> node_children = node->children();
  std::vector<const MapNode *>::const_iterator child = node_children.begin();
  for ( ; child != node_children.end(); ++child) {
    draw_node(*child, x_offset_direction, painter);
  }
}

void FreeMindDrawer::lay_out_map(const Map *map) {
  if (!node_drawing_info_.empty()) {
    return;
  }
  const MapNode *map_root = map->root();
  int root_y_coordinate_by_right_subtree = 0;
  int root_y_coordinate_by_left_subtree = 0;
  int root_right_children_number = 0;
  int root_left_children_number = 0;
  QFont qt_root_font("Arial", 12);
  QFontMetrics qt_font_metrics(qt_root_font);
  QString root_text = map_root->text().c_str();
  QSize qt_font_size = qt_font_metrics.size(Qt::TextExpandTabs, root_text);
  node_drawing_info_[map_root].width = qt_font_size.width();
  node_drawing_info_[map_root].height = qt_font_size.height();
  node_drawing_info_[map_root].qt_font = qt_root_font;
  std::vector<const MapNode *> root_children = map_root->children();
  std::vector<const MapNode *>::const_iterator child = root_children.begin();
  for ( ; child != root_children.end(); ++child) {
    NodePosition x_offset_direction = (*child)->position();
    lay_out_node(*child, map_root, x_offset_direction);
    if (x_offset_direction == RIGHT) {
      root_y_coordinate_by_right_subtree += node_drawing_info_[*child].coordinates.y;
      ++root_right_children_number;
    } else {
      root_y_coordinate_by_left_subtree += node_drawing_info_[*child].coordinates.y;
      ++root_left_children_number;
    }
  }  
  root_y_coordinate_by_right_subtree /= root_right_children_number;
  root_y_coordinate_by_left_subtree /= root_left_children_number;
  if (root_y_coordinate_by_left_subtree < root_y_coordinate_by_right_subtree) {
    left_subtree_y_offset_ = root_y_coordinate_by_right_subtree - 
                             root_y_coordinate_by_left_subtree;
    node_drawing_info_[map_root].coordinates.y = root_y_coordinate_by_right_subtree;
  } else {
    right_subtree_y_offset_ = root_y_coordinate_by_left_subtree - 
                              root_y_coordinate_by_right_subtree;
    node_drawing_info_[map_root].coordinates.y = root_y_coordinate_by_left_subtree;
  }
}

void FreeMindDrawer::lay_out_node(const MapNode *node, 
                                  const MapNode *parent, 
                                  NodePosition x_offset_direction) {
  if (node_drawing_info_.find(node) == node_drawing_info_.end()) {
    node_drawing_info_.insert(std::pair<const MapNode *, NodeDrawingInfo>(node, NodeDrawingInfo()));
    QFont qt_node_font("Arial", 12);
    QFontMetrics qt_font_metrics(qt_node_font);
    QString node_text = node->text().c_str();
    QSize qt_font_size = qt_font_metrics.size(Qt::TextExpandTabs, node_text);
    node_drawing_info_[node].width = qt_font_size.width();
    node_drawing_info_[node].height = qt_font_size.height();
    node_drawing_info_[node].qt_font = qt_node_font;
  }
  
  int x = 0;
  if (x_offset_direction == RIGHT) {
    x = node_drawing_info_[parent].coordinates.x + 
        node_drawing_info_[parent].width + x_step_;
  } else {
    x = node_drawing_info_[parent].coordinates.x -  
        node_drawing_info_[node].width - x_step_;
  }
  node_drawing_info_[node].coordinates.x = x;
  int node_width = node_drawing_info_[node].width;
  if (x < left_top_map_point.x) {
    left_top_map_point.x = x;
  } else if (x + node_width > right_bottom_map_point.x) {
    right_bottom_map_point.x = x + node_width;
  }
  
  std::vector<const MapNode *> node_children = node->children();
  std::vector<const MapNode *>::const_iterator child = node_children.begin();
  for ( ; child != node_children.end(); ++child) {
    lay_out_node(*child, node, x_offset_direction);
  }

  int y = 0;
  if (node->children().empty()) {
    if (x_offset_direction == RIGHT) {
      y = right_subtree_max_y_ + node_drawing_info_[node].height + y_step_;
      right_subtree_max_y_ = y;
    } else {
      y = left_subtree_max_y_ + node_drawing_info_[node].height + y_step_;
      left_subtree_max_y_ = y;
    }   
  } else {
    child = node_children.begin();
    for ( ; child != node_children.end(); ++child) {
      y += node_drawing_info_[*child].coordinates.y;
    }
    y /= node_children.size();
  }
  node_drawing_info_[node].coordinates.y = y;
  int node_height = node_drawing_info_[node].height;
  if (y + node_height > right_bottom_map_point.y) {
    right_bottom_map_point.y = y + node_height;
  }
}