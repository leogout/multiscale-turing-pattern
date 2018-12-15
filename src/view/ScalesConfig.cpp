//
// Created by leogout on 12/12/18.
//

#include <QtWidgets/QVBoxLayout>
#include "ScalesConfig.h"
#include "ScaleConfigWidget.h"


ScalesConfig::ScalesConfig(std::vector<ScaleConfig> &scalesConfig) {
  auto config_layout = new QVBoxLayout;

  for (auto &sc : scalesConfig) {
    auto scw = new ScaleConfigWidget(sc);
    m_scales_config.push_back(scw);
    config_layout->addWidget(scw);
  }

  setLayout(config_layout);
}

void ScalesConfig::setConfig(std::vector<ScaleConfig> scalesConfig) {
  for (int i = 0; i < scalesConfig.size(); ++i) {
    m_scales_config[i]->setConfig(scalesConfig[i]);
  }
}

std::vector<ScaleConfig> ScalesConfig::getConfig() {
  std::vector<ScaleConfig> scs;

  for (auto &sc : m_scales_config) {
    scs.push_back(sc->getConfig());
  }

  return scs;
}

