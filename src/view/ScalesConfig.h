//
// Created by leogout on 12/12/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_SCALESCONFIG_H
#define MULTISCALE_TURING_PATTERN_CPP_SCALESCONFIG_H

#include <QtCore/QArgument>
#include <QtWidgets/QWidget>
#include <src/ScaleConfig.h>
#include "ScaleConfigWidget.h"

class ScalesConfig : public QWidget
{
    Q_OBJECT
public:
    std::vector<ScaleConfigWidget*> m_scales_config;

    explicit ScalesConfig(std::vector<ScaleConfig> &config);
    void setConfig(std::vector<ScaleConfig> &scalesConfig);
    std::vector<ScaleConfig> getConfig();
};

#endif //MULTISCALE_TURING_PATTERN_CPP_SCALESCONFIG_H
