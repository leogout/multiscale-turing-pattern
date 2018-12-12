#include "ScaleConfigWidget.h"

#include <iostream>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

ScaleConfigWidget::ScaleConfigWidget(ScaleConfig &scale): m_scale(&scale) {
  auto layout = new QVBoxLayout;

  auto small_amounts_layout = new QHBoxLayout;
  auto small_amounts_label = new QLabel("Small amounts:");
  m_small_amounts_slider = new QSlider(Qt::Orientation::Horizontal);
  m_small_amounts_slider->setMaximum(30);
  m_small_amounts_slider->setValue(static_cast<int>(m_scale->sa * 100));
  m_small_amounts_value = new QLabel(QString::number(m_scale->sa, 'f', 2));

  auto activator_radius_layout = new QHBoxLayout;
  auto activator_radius_label = new QLabel("Activator radius:");
  m_activator_radius_input = new QSpinBox;
  m_activator_radius_input->setMaximum(500);
  m_activator_radius_input->setValue(m_scale->aR);

  auto inhibitor_radius_layout = new QHBoxLayout;
  auto inhibitor_radius_label = new QLabel("Inhibitor radius:");
  m_inhibitor_radius_input = new QSpinBox;
  m_inhibitor_radius_input->setMaximum(500);
  m_inhibitor_radius_input->setValue(m_scale->iR);

  auto color_layout = new QHBoxLayout;
  m_color_button = new QPushButton("Select color");
  m_color_label = new QLabel("Color <span style=\"background-color:" + scale.color.name() + "; color:" + scale.color.name() + ";\">.....</span>");

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
      double value = (m_small_amounts_slider->value() + 1) / 100.0;
      m_small_amounts_value->setText(QString::number(value, 'f', 2));
      m_scale->sa = value;
  });
  connect(m_activator_radius_input, qOverload<int>(&QSpinBox::valueChanged), [this]{
      m_scale->aR = m_activator_radius_input->value();
  });
  connect(m_inhibitor_radius_input, qOverload<int>(&QSpinBox::valueChanged), [this]{
      m_scale->iR = m_inhibitor_radius_input->value();
  });
  connect(m_color_button, &QPushButton::pressed, [this]{
      m_color = QColorDialog::getColor(Qt::white, this, "Select scale color");
      m_color_label->setText("Color <span style=\"background-color:" + m_color.name() + "; color:" + m_color.name() + ";\">.....</span>");
  });
}

ScaleConfig ScaleConfigWidget::getScaleConfig() {
  return ScaleConfig(
      m_activator_radius_input->value(),
      m_inhibitor_radius_input->value(),
      m_small_amounts_slider->value(),
      m_color
  );
}
