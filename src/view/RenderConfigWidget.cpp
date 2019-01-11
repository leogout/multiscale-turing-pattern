//
// Created by leogout on 07/12/18.
//

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "RenderConfigWidget.h"

RenderConfigWidget::RenderConfigWidget(RenderConfig &rc) {
  auto renderconfig_layout = new QVBoxLayout;

  m_width_input = new QSpinBox;
  m_width_input->setMaximum(10000);
  m_width_input->setValue(rc.width);

  m_height_input = new QSpinBox;
  m_height_input->setMaximum(10000);
  m_height_input->setValue(rc.height);

  m_pass_input = new QSpinBox;
  m_pass_input->setMaximum(1000);
  m_pass_input->setValue(rc.passes);

  auto size_label = new QLabel("Size:");
  auto pass_label = new QLabel("Passes:");

  auto pass_layout = new QHBoxLayout;
  auto size_layout = new QHBoxLayout;

  pass_layout->addWidget(pass_label);
  pass_layout->addWidget(m_pass_input);
  renderconfig_layout->addLayout(pass_layout);

  size_layout->addWidget(size_label);
  size_layout->addWidget(m_width_input);
  size_layout->addWidget(m_height_input);
  renderconfig_layout->addLayout(size_layout);

  setLayout(renderconfig_layout);
}

void RenderConfigWidget::setConfig(RenderConfig &rc) {
  m_pass_input->setValue(rc.passes);
  m_width_input->setValue(rc.width);
  m_height_input->setValue(rc.height);
}

RenderConfig RenderConfigWidget::getConfig() {
  return RenderConfig(
      m_pass_input->value(),
      m_width_input->value(),
      m_height_input->value()
  );
}

