//
// Created by leogout on 09/11/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_PAINTAREA_H
#define MULTISCALE_TURING_PATTERN_CPP_PAINTAREA_H


#include <QWidget>

class PaintArea : public QWidget {

public:
    std::vector<QColor> m_image;
    bool repaint = false;

    PaintArea(QWidget *parent = 0);
    void setImage(std::vector<QColor> &image);

protected:
    void paintEvent(QPaintEvent *event);
};


#endif //MULTISCALE_TURING_PATTERN_CPP_PAINTAREA_H
