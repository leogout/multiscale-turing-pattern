//
// Created by leogout on 07/12/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_RENDERCONFIGWIDGET_H
#define MULTISCALE_TURING_PATTERN_CPP_RENDERCONFIGWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include "src/RenderConfig.h"

class RenderConfigWidget : public QWidget
{
    Q_OBJECT
public:
    RenderConfig *m_rc;
    explicit RenderConfigWidget(RenderConfig &scale);
private:
    QSpinBox* m_width_input;
    QSpinBox* m_height_input;
    QSpinBox* m_pass_input;
};


#endif //MULTISCALE_TURING_PATTERN_CPP_RENDERCONFIGWIDGET_H
