//
// Created by leogout on 09/11/18.
//

#include <QtGui/QPainter>
#include <iostream>
#include "PaintArea.h"

PaintArea::PaintArea(QWidget *parent) {

}

void PaintArea::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  if (repaint) {
    std::cout << "Painting" << std::endl;

    for (int i = 0; i < m_image.size(); ++i) {
      painter.setPen(m_image[i]);
      painter.drawPoint(i % 100, i / 100);
    }
  }
}

void PaintArea::setImage(std::vector<QColor> &image) {
  m_image = image;
}
