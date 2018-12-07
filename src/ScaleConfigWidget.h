//
// Created by leogout on 09/11/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_MAINWINDOW_H
#define MULTISCALE_TURING_PATTERN_CPP_MAINWINDOW_H


#include <QPushButton>
#include <QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSlider>
#include "ScaleConfig.h"

class ScaleConfigWidget : public QWidget
{
Q_OBJECT
public:
    ScaleConfig *m_scale;
    explicit ScaleConfigWidget(ScaleConfig &scale);
private:
    QSlider* m_small_amounts_slider;
    QLabel* m_small_amounts_value;
    QSpinBox* m_activator_radius_input;
    QSpinBox* m_inhibitor_radius_input;
    QSpinBox* m_color_r_edit;
    QSpinBox* m_color_g_edit;
    QSpinBox* m_color_b_edit;
};


#endif //MULTISCALE_TURING_PATTERN_CPP_MAINWINDOW_H
