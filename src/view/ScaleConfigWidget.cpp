#include "ScaleConfigWidget.h"

#include <iostream>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

ScaleConfigWidget::ScaleConfigWidget(ScaleConfig &scale) {
  auto layout = new QVBoxLayout;

  auto small_amounts_layout = new QHBoxLayout;
  auto small_amounts_label = new QLabel("Small amounts:");
  m_small_amounts_slider = new QSlider(Qt::Orientation::Horizontal);
  m_small_amounts_slider->setMinimum(1);
  m_small_amounts_slider->setMaximum(10);
  m_small_amounts_slider->setValue(static_cast<int>(scale.sa * 100));
  m_small_amounts_value = new QLabel(QString::number(scale.sa, 'f', 2));

  auto activator_radius_layout = new QHBoxLayout;
  auto activator_radius_label = new QLabel("Activator radius:");
  m_activator_radius_input = new QSpinBox;
  m_activator_radius_input->setMaximum(500);
  m_activator_radius_input->setValue(scale.aR);

  auto inhibitor_radius_layout = new QHBoxLayout;
  auto inhibitor_radius_label = new QLabel("Inhibitor radius:");
  m_inhibitor_radius_input = new QSpinBox;
  m_inhibitor_radius_input->setMaximum(500);
  m_inhibitor_radius_input->setValue(scale.iR);

  auto color_layout = new QHBoxLayout;
  m_color_button = new QPushButton("Select color");
  m_color_label = new QLabel("Color <span style=\"background-color:" + scale.color.name() + "; color:" + scale.color.name() + ";\">.....</span>");
  m_color = scale.color;


  small_amounts_layout->addWidget(small_amounts_label);
  small_amounts_layout->addWidget(m_small_amounts_slider);
  small_amounts_layout->addWidget(m_small_amounts_value);

  activator_radius_layout->addWidget(activator_radius_label);
  activator_radius_layout->addWidget(m_activator_radius_input);

  inhibitor_radius_layout->addWidget(inhibitor_radius_label);
  inhibitor_radius_layout->addWidget(m_inhibitor_radius_input);

  color_layout->addWidget(m_color_label);
  color_layout->addWidget(m_color_button);

  layout->addLayout(small_amounts_layout);
  layout->addLayout(activator_radius_layout);
  layout->addLayout(inhibitor_radius_layout);
  layout->addLayout(color_layout);

  setLayout(layout);

  connect(m_small_amounts_slider, &QSlider::valueChanged, [this]{
      double value = m_small_amounts_slider->value() / 100.0;
      m_small_amounts_value->setText(QString::number(value, 'f', 2));
  });
  connect(m_color_button, &QPushButton::pressed, [this]{
      m_color = QColorDialog::getColor(Qt::white, this, "Select scale color");
      m_color_label->setText("Color <span style=\"background-color:" + m_color.name() + "; color:" + m_color.name() + ";\">.....</span>");
  });
}

void ScaleConfigWidget::setConfig(ScaleConfig &config) {
  m_activator_radius_input->setValue(config.aR);
  m_inhibitor_radius_input->setValue(config.iR);
  m_small_amounts_slider->setValue(static_cast<int>(config.sa * 100));
  m_color = config.color;
  m_color_label->setText("Color <span style=\"background-color:" + m_color.name() + "; color:" + m_color.name() + ";\">.....</span>");
}

ScaleConfig ScaleConfigWidget::getConfig() {
  return ScaleConfig(
      m_activator_radius_input->value(),
      m_inhibitor_radius_input->value(),
      m_small_amounts_slider->value() / 100.0,
      m_color
  );
}
