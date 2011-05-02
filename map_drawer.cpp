#include "QtCore/Qt"
#include "QtCore/QString"
#include "QtCore/QPoint"
#include "QtCore/QPointF"
#include "QtCore/QRect"
#include "QtGui/QImage"
#include "QtGui/QColor"
#include "QtGui/QBrush"
#include "QtGui/QPen"
#include "QtGui/QPainterPath"
#include "QtGui/QPolygonF"
#include "QtGui/QFontMetrics"
#include "map_drawer.h"
#include "map_settings.h"

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
    : qt_apprication_(argc, argv),
      map_width_ (0),
      map_height_ (0),
      x_step_(30),
      y_step_(20), 
      right_subtree_max_y_ (0),
      left_subtree_max_y_ (0),
      right_subtree_y_offset_ (0),
      left_subtree_y_offset_ (0),
      border_indent_ (40),
      node_text_indent_ (5),
      straight_arrow_link_border_indent_ (20),
      straight_arrow_link_dividing_indent_ (2),
      arrowhead_height_ (15),
      arrowhead_widht_ (7),
      curved_edge_control_point_indent_ (x_step_ / 2),
      bubble_node_rounded_rect_radius_ (5),
      icon_size_ (16) {}

FreeMindDrawer::NodeDrawingInfo::NodeDrawingInfo() 
    : width(0),
      height(0) {}

void FreeMindDrawer::draw(const Map *map) {
  lay_out_map(map);
  QImage *map_image = new QImage(map_width_, map_height_, QImage::Format_RGB444);
  QPainter map_painter(map_image);
  QBrush map_background_brush(Qt::white, Qt::SolidPattern);
  map_painter.fillRect(0, 0, map_width_, map_height_, map_background_brush);
  map_painter.setBackground(map_background_brush);
  map_painter.setBrush(Qt::white);
  map_painter.setPen(Qt::black);
  draw_arrow_links(map, map->root(), map_painter);
  draw_node(map, map->root(), map_painter);
  map_painter.end();

  MapSettings *map_settings = MapSettings::get_map_settings();
  std::string output_file_name = map_settings->output_file_name();
  std::string output_file_format = map_settings->output_file_format();
  map_image->save(output_file_name.c_str(), output_file_format.c_str());
  delete map_image;
}

void FreeMindDrawer::draw_node(const Map *map, const MapNode *node, 
                               QPainter &painter) {
  int x = node_drawing_info_[node].coordinates.x;
  int y = node_drawing_info_[node].coordinates.y;
  int node_width = node_drawing_info_[node].width;
  int node_height = node_drawing_info_[node].height;
  QString node_text = node->text().c_str();
  QFont qt_node_font = node_drawing_info_[node].qt_font;
  painter.setFont(qt_node_font);
  MapSettings *map_settings = MapSettings::get_map_settings();
  std::string node_style = map_settings->node_style();
  if (node == map->root()) {
    int ellipse_left = x;
    int ellipse_top = y - node_height * 1.5;
    int ellipse_width = node_width;
    int ellipse_height = 2 * node_height;
    painter.drawEllipse(ellipse_left, ellipse_top, ellipse_width, ellipse_height);    
  } else {
    if (node_style == "BUBBLE") {
      QBrush qt_old_brush = painter.brush();
      if (!node->background_color().empty()) {
        QColor qt_background_color(node->background_color().c_str());
        QBrush qt_background_brush(qt_background_color);
        painter.setBrush(qt_background_brush);
      }
      painter.drawRoundedRect(x, y - node_height, node_width, node_height, 
                              bubble_node_rounded_rect_radius_, 
                              bubble_node_rounded_rect_radius_);
      painter.setBrush(qt_old_brush);
    }
    else if (node_style == "LINEAR") {
      if (!node->background_color().empty()) {
        QColor qt_background_color(node->background_color().c_str());
        QBrush qt_background_brush(qt_background_color);
        painter.fillRect(x, y - node_height, node_width, node_height, 
                         qt_background_brush);
      }
      painter.drawLine(x, y, x + node_width, y);
    }
  }
  NodePosition node_position = node->position();
  std::vector<std::string> icons = node->icons();
  std::vector<std::string>::iterator icon_iterator = icons.begin();
  int icons_number = icons.size();
  int icon_x = 0;
  int icon_y = y - node_height / 2 - icon_size_ / 2;
  if (node_position == LEFT) {
    icon_x = x + node_width - node_text_indent_ - icons_number * icon_size_;
  } else {
    icon_x = x + node_text_indent_;
  }
  for ( ; icon_iterator != icons.end(); ++icon_iterator) {
    std::string icon_name = "icons/" + *icon_iterator;
    QImage icon(icon_name.c_str());
    painter.drawImage(icon_x, icon_y, icon);
    icon_x += icon_size_;
  } 
  int indents_number = 1;
  if (icons_number != 0) {
    ++indents_number;
  }

  QFontMetrics qt_node_font_metrics(qt_node_font);
  int text_rect_left = 0;
  int text_rect_top = y - node_height + node_text_indent_;
  int text_rect_right = 0;
  int text_rect_bottom = y + node_text_indent_;
  if (node_position == LEFT) {
    text_rect_left = x + node_text_indent_;
    text_rect_right = x + node_width - indents_number * node_text_indent_ - icons_number * icon_size_;;
  } else {
    text_rect_left = x + indents_number * node_text_indent_ + icons_number * icon_size_;
    text_rect_right = x + node_width - node_text_indent_;
  }
  QPoint qt_text_rect_left_top(text_rect_left, text_rect_top);
  QPoint qt_text_rect_right_bottom(text_rect_right, text_rect_bottom);
  QRect qt_text_rect(qt_text_rect_left_top, qt_text_rect_right_bottom);
  QColor qt_text_color;
  QPen qt_text_pen;
  QPen qt_old_pen = painter.pen();
  if (!node->text_color().empty()) {
    QString qt_text_color_name = node->text_color().c_str();
    qt_text_color.setNamedColor(qt_text_color_name);
    qt_text_pen.setColor(qt_text_color);
    painter.setPen(qt_text_color);
  } else {
    painter.setPen(Qt::black);
  }
  painter.drawText(qt_text_rect, Qt::AlignLeft, node_text);
  painter.setPen(qt_old_pen);

  std::string edge_style = map_settings->edge_style();
  std::vector<const MapNode *> node_children = node->children();
  std::vector<const MapNode *>::const_iterator child = node_children.begin();
  for ( ; child != node_children.end(); ++child) {
    draw_node(map, *child, painter);
    if (edge_style == "STRAIGHT") {
      if (node_style == "BUBBLE") {
        draw_straight_edge(map, node, *child, CENTER, painter);
      } else if (node_style == "LINEAR") {
        draw_straight_edge(map, node, *child, BOTTOM, painter);    
      }
    } else if (edge_style == "CURVED") {
      if (node_style == "BUBBLE") {
        draw_curved_edge(map, node, *child, CENTER, painter);
      } else if (node_style == "LINEAR") {
        draw_curved_edge(map, node, *child, BOTTOM, painter);
      }
    }
  }
}

void FreeMindDrawer::draw_arrow_links(const Map *map, const MapNode *node, QPainter &painter) {
  std::vector<ArrowLink> arrow_links = node->arrow_links();
  std::vector<ArrowLink>::const_iterator arrow_link_iterator = arrow_links.begin();
  for ( ; arrow_link_iterator != arrow_links.end(); ++arrow_link_iterator) {
    std::string destination_node_id = arrow_link_iterator->destination_node_id;
    const MapNode *destination_node = map->find_node_by_id(destination_node_id);
    if (destination_node != NULL) {
      draw_straight_arrow_link(map, node, destination_node, *arrow_link_iterator, painter);
    }
  }
  std::vector<const MapNode *> node_children = node->children();
  std::vector<const MapNode *>::const_iterator child = node_children.begin();
  for ( ; child != node_children.end(); ++child) {
    draw_arrow_links(map, *child, painter);
  }
}

void FreeMindDrawer::draw_straight_edge(const Map *map, const MapNode *parent, 
                                        const MapNode *child,  EdgeBinding edge_binding,
                                        QPainter &painter) {
  int parent_x = node_drawing_info_[parent].coordinates.x;
  int child_x = node_drawing_info_[child].coordinates.x;
  int start_x = parent_x;
  int start_y = node_drawing_info_[parent].coordinates.y;
  int end_x = child_x;
  int end_y = node_drawing_info_[child].coordinates.y;
  
  if (parent_x < child_x) {
    int parent_width = node_drawing_info_[parent].width;
    start_x += parent_width;
  } else {
    int child_width = node_drawing_info_[child].width;
    end_x += child_width;
  }
  if (edge_binding == CENTER || parent == map->root()) {
    int parent_height = node_drawing_info_[parent].height;
    start_y -= parent_height / 2;
  }
  if (edge_binding == CENTER) {
    int child_height = node_drawing_info_[child].height;
    end_y -= child_height / 2;
  }
  painter.drawLine(start_x, start_y, end_x, end_y);
}

void FreeMindDrawer::draw_curved_edge(const Map *map, const MapNode *parent, 
                                      const MapNode *child,  EdgeBinding edge_binding,
                                      QPainter &painter) {
  int parent_x = node_drawing_info_[parent].coordinates.x;
  int child_x = node_drawing_info_[child].coordinates.x;
  int start_x = parent_x;
  int start_y = node_drawing_info_[parent].coordinates.y;
  int end_x = child_x;
  int end_y = node_drawing_info_[child].coordinates.y;
  int parent_control_x = 0;
  int patenr_control_y = 0;
  int child_control_x = 0;
  int child_control_y = 0;

  if (parent_x < child_x) {
    int parent_width = node_drawing_info_[parent].width;
    start_x += parent_width;
    parent_control_x = start_x + curved_edge_control_point_indent_;
    child_control_x = end_x - curved_edge_control_point_indent_;
  } else {
    int child_width = node_drawing_info_[child].width;
    end_x += child_width;
    parent_control_x = start_x - curved_edge_control_point_indent_;
    child_control_x = end_x + curved_edge_control_point_indent_;
  }
  if (edge_binding == CENTER || parent == map->root()) {
    int parent_height = node_drawing_info_[parent].height;
    start_y -= parent_height / 2;
  }
  if (edge_binding == CENTER) {
    int child_height = node_drawing_info_[child].height;
    end_y -= child_height / 2;
  }
  patenr_control_y = start_y;
  child_control_y = end_y;
  QPainterPath qt_path;
  qt_path.moveTo(start_x, start_y);
  qt_path.cubicTo(parent_control_x, patenr_control_y, 
                  child_control_x, child_control_y, end_x, end_y);
  QBrush qt_old_brush = painter.brush();
  painter.setBrush(Qt::NoBrush);
  painter.drawPath(qt_path);
  painter.setBrush(qt_old_brush);
}

void FreeMindDrawer::draw_straight_arrow_link(const Map *map, const MapNode *start_node, 
                                        const MapNode *end_node, ArrowLink arrow_link, 
                                        QPainter &painter) {
  int start_node_x = node_drawing_info_[start_node].coordinates.x;
  int start_node_y = node_drawing_info_[start_node].coordinates.y;
  int start_node_height = node_drawing_info_[start_node].height;
  int start_node_wigth = node_drawing_info_[start_node].width;
  int end_node_x = node_drawing_info_[end_node].coordinates.x;
  int end_node_y = node_drawing_info_[end_node].coordinates.y;
  int end_node_height = node_drawing_info_[end_node].height;
  int end_node_width = node_drawing_info_[end_node].width;
  int start_x = 0;
  int start_y = 0;
  int end_x = 0;
  int end_y = 0;
  static int arrow_link_count = 0;
  NodePosition start_node_position = start_node->position();
  NodePosition end_node_position = end_node->position();

  ++arrow_link_count;
  if (start_node_position == LEFT) {
    start_x = start_node_x;
  } else {
    start_x = start_node_x + start_node_wigth;
  }
  if (end_node_position == LEFT) {
    end_x = end_node_x;
  } else {
    end_x = end_node_x + end_node_width;
  }
  start_y = start_node_y - start_node_height / 2;
  end_y = end_node_y - end_node_height / 2;
  QPainterPath qt_path;
  qt_path.moveTo(start_x, start_y);
  if (start_node_position == LEFT && end_node_position == LEFT) {
    int intermediate_point_x = straight_arrow_link_border_indent_ - 
                               arrow_link_count * straight_arrow_link_dividing_indent_;
    int intermediate_point_y = start_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
    intermediate_point_y = end_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
  } else if (start_node_position == LEFT && end_node_position != LEFT) {
    int intermediate_point_x = straight_arrow_link_border_indent_ - 
                               arrow_link_count * straight_arrow_link_dividing_indent_;
    int intermediate_point_y = start_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
    if (start_y <= map_height_ / 2) {
      intermediate_point_y = straight_arrow_link_border_indent_ - 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);
      intermediate_point_x = map_width_ - straight_arrow_link_border_indent_ + 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);
    } else {
      intermediate_point_y = map_height_ - straight_arrow_link_border_indent_ + 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);
      intermediate_point_x = map_width_ - straight_arrow_link_border_indent_ + 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);      
    }
    intermediate_point_y = end_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
  } else if (start_node_position != LEFT && end_node_position == LEFT) {
    int intermediate_point_x = map_width_ - straight_arrow_link_border_indent_ + 
                               arrow_link_count * straight_arrow_link_dividing_indent_;
    int intermediate_point_y = start_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
    if (start_y <= map_height_ / 2) {
      intermediate_point_y = straight_arrow_link_border_indent_ - 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);
      intermediate_point_x = straight_arrow_link_border_indent_ - 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);
    } else {
      intermediate_point_y = map_height_ - straight_arrow_link_border_indent_ + 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);
      intermediate_point_x = straight_arrow_link_border_indent_ - 
                             arrow_link_count * straight_arrow_link_dividing_indent_;
      qt_path.lineTo(intermediate_point_x, intermediate_point_y);      
    }
    intermediate_point_y = end_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
  } else {
    int intermediate_point_x = map_width_ - straight_arrow_link_border_indent_ + 
                               arrow_link_count * straight_arrow_link_dividing_indent_;
    int intermediate_point_y = start_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
    intermediate_point_y = end_y;
    qt_path.lineTo(intermediate_point_x, intermediate_point_y);
  }
  qt_path.lineTo(end_x, end_y); 
  QPen qt_old_pen = painter.pen();
  QBrush qt_old_brush = painter.brush();
  if (!arrow_link.color.empty()) {
    QColor qt_arrow_color(arrow_link.color.c_str());
    QPen qt_arrow_pen(qt_arrow_color);
    painter.setPen(qt_arrow_pen);
    painter.setBrush(Qt::NoBrush);
  }
  painter.drawPath(qt_path);
  if (arrow_link.start_arrow || arrow_link.end_arrow) {
    if (!arrow_link.color.empty()) {
      QColor qt_arrow_color(arrow_link.color.c_str());
      QBrush qt_arrowhead_brush(qt_arrow_color);
      painter.setBrush(qt_arrowhead_brush);
    } else {
      painter.setBrush(Qt::SolidPattern);
    }
  }
  if (arrow_link.start_arrow) {
    QPolygonF qt_arrowhead;
    qt_arrowhead << QPointF(start_x, start_y);
    if (start_node_position == LEFT) {
      qt_arrowhead << QPointF(start_x - arrowhead_height_, start_y - arrowhead_widht_ / 2);
      qt_arrowhead << QPointF(start_x - arrowhead_height_, start_y + arrowhead_widht_ / 2);
    } else {
      qt_arrowhead << QPointF(start_x + arrowhead_height_, start_y - arrowhead_widht_ / 2);
      qt_arrowhead << QPointF(start_x + arrowhead_height_, start_y + arrowhead_widht_ / 2);    
    }
    qt_arrowhead << QPointF(start_x, start_y);
    painter.drawPolygon(qt_arrowhead);  
  }
  if (arrow_link.end_arrow) {
    QPolygonF qt_arrowhead;
    qt_arrowhead << QPointF(end_x, end_y);
    if (end_node_position == LEFT) {
      qt_arrowhead << QPointF(end_x - arrowhead_height_, end_y - arrowhead_widht_ / 2);
      qt_arrowhead << QPointF(end_x - arrowhead_height_, end_y + arrowhead_widht_ / 2);
    } else {
      qt_arrowhead << QPointF(end_x + arrowhead_height_, end_y - arrowhead_widht_ / 2);
      qt_arrowhead << QPointF(end_x + arrowhead_height_, end_y + arrowhead_widht_ / 2);    
    }
    qt_arrowhead << QPointF(end_x, end_y);
    painter.drawPolygon(qt_arrowhead);
  }
  painter.setBrush(qt_old_brush);
  painter.setPen(qt_old_pen);
}

void FreeMindDrawer::draw_curved_arrow_link(const Map *map, const MapNode *start_node, 
                                     const MapNode *end_node, ArrowLink arrow_link, 
                                     QPainter &painter) {
  int start_node_x = node_drawing_info_[start_node].coordinates.x;
  int start_node_y = node_drawing_info_[start_node].coordinates.y;
  int start_node_height = node_drawing_info_[start_node].height;
  int start_node_wigth = node_drawing_info_[start_node].width;
  int end_node_x = node_drawing_info_[end_node].coordinates.x;
  int end_node_y = node_drawing_info_[end_node].coordinates.y;
  int end_node_height = node_drawing_info_[end_node].height;
  int end_node_width = node_drawing_info_[end_node].width;
  int start_x = start_node_x + start_node_wigth / 2;
  int start_y = start_node_y - start_node_height / 2;
  int end_x = end_node_x + end_node_width / 2;
  int end_y = end_node_y - end_node_height / 2;
  int start_control_x = 0;
  int start_control_y = 0;
  int end_control_x = 0;
  int end_control_y = 0;

  if (start_node->position() == RIGHT) {
    start_control_x = start_x + arrow_link.start_inclination.x;
  } else {
    start_control_x = start_x - arrow_link.start_inclination.x;
  }
  if (end_node->position() == RIGHT) {
    end_control_x = end_x + arrow_link.end_inclination.x;
  } else {
    end_control_x = end_x - arrow_link.end_inclination.x;
  }
  start_control_y = start_y + arrow_link.start_inclination.y;
  end_control_y = end_y + arrow_link.end_inclination.y;
  QPainterPath qt_path;
  qt_path.moveTo(start_x, start_y);
  qt_path.cubicTo(start_control_x, start_control_y, 
                  end_control_x, end_control_y, end_x, end_y);
  painter.drawPath(qt_path);
}

void FreeMindDrawer::lay_out_map(const Map *map) {
  if (!node_drawing_info_.empty()) {
    return;
  }
  const MapNode *map_root = map->root();
  int root_y_coordinate_by_right_subtree = 0;
  int root_y_coordinate_by_left_subtree = 0;
  NodeTextFont root_font = map_root->font();
  QFont qt_root_font(root_font.name.c_str(), root_font.size);
  qt_root_font.setBold(root_font.is_bold);
  qt_root_font.setItalic(root_font.is_italic);
  QFontMetrics qt_font_metrics(qt_root_font);
  QString root_text = map_root->text().c_str();
  QSize qt_font_size = qt_font_metrics.size(Qt::TextExpandTabs, root_text);
  int icons_number = map_root->icons().size();
  int indents_number = 2;
  if (icons_number != 0) {
    ++indents_number;
  }
  node_drawing_info_[map_root].width = qt_font_size.width() + 
                                       indents_number * node_text_indent_ + 
                                       icons_number * icon_size_;
  node_drawing_info_[map_root].height = qt_font_size.height() + 
                                        qt_font_metrics.descent() + 
                                        2 * node_text_indent_;
  node_drawing_info_[map_root].qt_font = qt_root_font;
  std::vector<const MapNode *> root_children = map_root->children();
  std::vector<const MapNode *>::const_iterator child = root_children.begin();
  for ( ; child != root_children.end(); ++child) {
    NodePosition x_offset_direction = (*child)->position();
    lay_out_node(*child, map_root, x_offset_direction);
  }  
  root_y_coordinate_by_right_subtree = right_subtree_max_y_ / 2;
  root_y_coordinate_by_left_subtree = left_subtree_max_y_ / 2;
  if (root_y_coordinate_by_left_subtree < root_y_coordinate_by_right_subtree) {
    left_subtree_y_offset_ = root_y_coordinate_by_right_subtree - 
                             root_y_coordinate_by_left_subtree;
    node_drawing_info_[map_root].coordinates.y = root_y_coordinate_by_right_subtree;
  } else {
    right_subtree_y_offset_ = root_y_coordinate_by_left_subtree - 
                              root_y_coordinate_by_right_subtree;
    node_drawing_info_[map_root].coordinates.y = root_y_coordinate_by_left_subtree;
  }
  update_map_drawing_info(map); 
}

void FreeMindDrawer::lay_out_node(const MapNode *node, 
                                  const MapNode *parent, 
                                  NodePosition x_offset_direction) {
  if (node_drawing_info_.find(node) == node_drawing_info_.end()) {
    node_drawing_info_.insert(std::pair<const MapNode *, NodeDrawingInfo>(node, NodeDrawingInfo()));
    NodeTextFont node_font = node->font();
    QFont qt_node_font(node_font.name.c_str(), node_font.size);
    qt_node_font.setBold(node_font.is_bold);
    qt_node_font.setItalic(node_font.is_italic);
    QFontMetrics qt_font_metrics(qt_node_font);
    QString node_text = node->text().c_str();
    QSize qt_font_size = qt_font_metrics.size(Qt::TextExpandTabs, node_text);
    int icons_number = node->icons().size();
    int indents_numer = 2;
    if (icons_number != 0) {
      ++indents_numer;
    }
    node_drawing_info_[node].width = qt_font_size.width() + 
                                     indents_numer * node_text_indent_ + 
                                     icons_number * icon_size_;
    node_drawing_info_[node].height = qt_font_size.height() + 
                                      qt_font_metrics.descent() + 
                                      2 * node_text_indent_;
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
  if (x < left_top_map_point_.x) {
    left_top_map_point_.x = x;
  } else if (x + node_width > right_bottom_map_point_.x) {
    right_bottom_map_point_.x = x + node_width;
  }
  
  std::vector<const MapNode *> node_children = node->children();
  std::vector<const MapNode *>::const_iterator child = node_children.begin();
  for ( ; child != node_children.end(); ++child) {
    lay_out_node(*child, node, x_offset_direction);
  }

  int y = 0;
  if (node->children().empty()) {
    if (x_offset_direction == RIGHT) {
      y = right_subtree_max_y_ + node_drawing_info_[node].height;
      if (right_subtree_max_y_ != 0) {
        y += y_step_;
      }
      right_subtree_max_y_ = y;
    } else {
      y = left_subtree_max_y_ + node_drawing_info_[node].height;
      if (left_subtree_max_y_ != 0) {
        y += y_step_;
      }
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
  if (y > right_bottom_map_point_.y) {
    right_bottom_map_point_.y = y;
  }
}

void FreeMindDrawer::update_map_drawing_info(const Map *map) {
  const MapNode *map_root = map->root();
  update_root_coordinates(map_root);

  std::vector<const MapNode *> root_children = map_root->children();
  std::vector<const MapNode *>::const_iterator child = root_children.begin();
  for ( ; child != root_children.end(); ++child) {
    NodePosition x_offset_direction = (*child)->position();    
    update_nodes_coordinates(*child, x_offset_direction); 
  }
  map_width_ = right_bottom_map_point_.x - left_top_map_point_.x + 2 * border_indent_;
  map_height_ = right_bottom_map_point_.y + 2 * border_indent_;
}

void FreeMindDrawer::update_nodes_coordinates(const MapNode *node, NodePosition x_offset_direction) {
  int x = node_drawing_info_[node].coordinates.x - left_top_map_point_.x + border_indent_;
  int y = node_drawing_info_[node].coordinates.y + border_indent_;
  if (x_offset_direction == RIGHT) {
    y += right_subtree_y_offset_;
  } else {
    y += left_subtree_y_offset_;
  }
  node_drawing_info_[node].coordinates.x = x;
  node_drawing_info_[node].coordinates.y = y;

  std::vector<const MapNode *> node_children = node->children();
  std::vector<const MapNode *>::const_iterator child = node_children.begin();
  for ( ; child != node_children.end(); ++child) {
    update_nodes_coordinates(*child, x_offset_direction);
  }
}

void FreeMindDrawer::update_root_coordinates(const MapNode *root) {
  int x = node_drawing_info_[root].coordinates.x - left_top_map_point_.x + border_indent_;
  int y = node_drawing_info_[root].coordinates.y + border_indent_;
  node_drawing_info_[root].coordinates.x = x;
  node_drawing_info_[root].coordinates.y = y;
}