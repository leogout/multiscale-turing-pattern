#include "StepConfig.h"
#include "PaintArea.h"
#include "Color.h"

#include <iostream>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QAction>

StepConfig::StepConfig(Scale &scale): m_scale(&scale) {
  QMap<QString, QObject*> map;

  auto layout = new QVBoxLayout;

  auto small_amounts_layout = new QHBoxLayout;
  auto small_amounts_label = new QLabel("Small amounts");
  m_small_amounts_slider = new QSlider(Qt::Orientation::Horizontal);
  m_small_amounts_slider->setValue(static_cast<int>(m_scale->sa * 100));
  m_small_amounts_value = new QLabel(QString::number(m_scale->sa, 'f', 2));

  auto activator_radius_layout = new QHBoxLayout;
  auto activator_radius_label = new QLabel("Activator radius");
  m_activator_radius_input = new QSpinBox;
  m_activator_radius_input->setMaximum(500);
  m_activator_radius_input->setValue(m_scale->aR);

  auto inhibitor_radius_layout = new QHBoxLayout;
  auto inhibitor_radius_label = new QLabel("Inhibitor radius");
  m_inhibitor_radius_input = new QSpinBox;
  m_inhibitor_radius_input->setMaximum(500);
  m_inhibitor_radius_input->setValue(m_scale->iR);

  auto color_layout = new QHBoxLayout;
  auto color_label = new QLabel("Color");
  m_color_r_edit = new QSpinBox();
  m_color_g_edit = new QSpinBox();
  m_color_b_edit = new QSpinBox();
  m_color_r_edit->setMaximum(255);
  m_color_g_edit->setMaximum(255);
  m_color_b_edit->setMaximum(255);
  m_color_r_edit->setValue(m_scale->color.red);
  m_color_g_edit->setValue(m_scale->color.green);
  m_color_b_edit->setValue(m_scale->color.blue);

  small_amounts_layout->addWidget(small_amounts_label);
  small_amounts_layout->addWidget(m_small_amounts_slider);
  small_amounts_layout->addWidget(m_small_amounts_value);

  activator_radius_layout->addWidget(activator_radius_label);
  activator_radius_layout->addWidget(m_activator_radius_input);

  inhibitor_radius_layout->addWidget(inhibitor_radius_label);
  inhibitor_radius_layout->addWidget(m_inhibitor_radius_input);

  color_layout->addWidget(color_label);
  color_layout->addWidget(m_color_r_edit);
  color_layout->addWidget(m_color_g_edit);
  color_layout->addWidget(m_color_b_edit);

  layout->addLayout(small_amounts_layout);
  layout->addLayout(activator_radius_layout);
  layout->addLayout(inhibitor_radius_layout);
  layout->addLayout(color_layout);

  setLayout(layout);

  setWindowTitle("Multi Scale Turing Pattern");
  setWindowIcon(QIcon(":/icon.png"));

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
  connect(m_color_r_edit, qOverload<int>(&QSpinBox::valueChanged), [this]{
      m_scale->color.red = static_cast<Color::byte>(m_color_r_edit->value());
  });
  connect(m_color_g_edit, qOverload<int>(&QSpinBox::valueChanged), [this]{
      m_scale->color.green = static_cast<Color::byte>(m_color_g_edit->value());
  });
  connect(m_color_b_edit, qOverload<int>(&QSpinBox::valueChanged), [this]{
      m_scale->color.blue = static_cast<Color::byte>(m_color_b_edit->value());
  });
}
