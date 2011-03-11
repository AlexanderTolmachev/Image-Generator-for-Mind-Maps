#include <QtGui/QImage>
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include "draw_map.h"

void SimpleDraw::draw(Map *map, int argv, char **args) {
  QApplication app(argv, args);
  QImage *Image = new QImage(200, 200, QImage::Format_RGB444);
  QBrush baseBrush(Qt::white, Qt::SolidPattern);
  QPainter painter(Image);
 
  painter.fillRect(0, 0, 200, 200, baseBrush);
  painter.setBackground(baseBrush);
  painter.setBrush(Qt::white);
  painter.setPen(Qt::black);

  QString text = map->map()->text().c_str();
  QFont font("Arial", 20);
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

