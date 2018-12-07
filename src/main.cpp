#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <random>
#include <QtCore>
#include <QtWidgets>
#include <QApplication>
#include <QTextStream>
#include <QFileInfo>

#include "PaintArea.h"
#include "ScaleConfig.h"
#include "RenderConfig.h"

int circular_index(int i, int j, int w, int h) {
  return (w + i) % w + ((h + j) % h) * w;
}

void blur_h(std::vector<double> &src, std::vector<double> &dest, int r, int w, int h) {
  double sum;
  int area = r*2 + 1;
  for (int y = 0; y < h; ++y) {
    sum = 0;
    for (int i = -r; i <= r; ++i) {
      sum += src[circular_index(i, y, w, h)];
    }
    for (int x = 0; x < w; ++x) {
      sum += src[circular_index(x + r, y, w, h)] - src[circular_index(x - r, y, w, h)];
      dest[circular_index(x, y, w, h)] = sum / area;
    }
  }
}

void blur_v(std::vector<double> &src, std::vector<double> &dest, int r, int w, int h) {
  double sum;
  int area = r*2 + 1;
  for (int x = 0; x < w; ++x) {
    sum = 0;
    for (int i = -r; i <= r; ++i) {
      sum += src[circular_index(x, i, w, h)];
    }
    for (int y = 0; y < h; ++y) {
      sum += src[circular_index(x, y + r, w, h)] - src[circular_index(x, y - r, w, h)];
      dest[circular_index(x, y, w, h)] = sum / area;
    }
  }
}

void blur(std::vector<double> &src, std::vector<double> &dest, std::vector<double> &buffer, int r, int w, int h) {
  blur_h(src, buffer, r, w, h);
  blur_v(buffer, dest, r, w, h);
}

void generate(RenderConfig &rc, std::vector<double> &main_grid, std::vector<Color> &colors, std::vector<Scale> &scales, QImage &image) {
  int width = rc.width;
  int height = rc.height;
  std::vector<double> buffer(width * height);

  for (int pass = 0; pass < rc.passes; ++pass) {
    std::cout << "Pass " << pass << std::endl;
    for (Scale &scale : scales) {
      blur(main_grid, scale.activator, buffer, scale.aR, width, height);
      blur(main_grid, scale.inhibitor, buffer, scale.iR, width, height);

      for (int i = 0; i < main_grid.size(); ++i) {
        scale.variation[i] = fabs(
            scale.activator[i] - scale.inhibitor[i]
        );
      }
    }

    double max = 0;
    double min = 0;

    for (int i = 0; i < main_grid.size(); ++i) {
      int best_scale = 0;
      for (int scale = 0; scale < scales.size(); scale++) {
        if (scales[scale].variation[i] < scales[best_scale].variation[i]) {
          best_scale = scale;
        }
      }

      double factor;

      if (scales[best_scale].activator[i] > scales[best_scale].inhibitor[i]) {
        factor = scales[best_scale].sa;
      } else {
        factor = -scales[best_scale].sa;
      }

      colors[i].bump_to(scales[best_scale].color, 0.1);
      main_grid[i] += factor;

      max = std::max(max, main_grid[i]);
      min = std::min(min, main_grid[i]);
    }

    // Normalization
    for (double &i : main_grid) {
      i = (i - min) / (max - min) * 2 - 1;
    }
  }

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      Color col = colors[i + j * width] * ((main_grid[i + j * width] + 1) / 2);
      image.setPixel(i, j, qRgb(col.red, col.green, col.blue));
    }
  }
}

void init(int width, int height, QImage &image, std::vector<double> &main_grid, std::vector<Color> &colors, std::vector<Scale> &scales) {
  std::random_device rd;
  std::mt19937 mt(1234);
  std::uniform_real_distribution<double> dist(-1, 1);

  std::cout << width << " " << height << std::endl;

  main_grid = std::vector<double>(width * height);
  colors = std::vector<Color>(width * height);
  image = QImage(width, height, QImage::Format_RGB888);
  scales = {
      Scale(width * height, 200, 50, 0.5, Color(237, 255, 143)),
      Scale(width * height, 100, 25, 0.4, Color(232, 204, 81)),
      Scale(width * height, 50, 10, 0.3, Color(255, 198, 116)),
      Scale(width * height, 25, 5, 0.2, Color(232, 133, 98)),
      Scale(width * height, 10, 2, 0.1, Color(255, 131, 162)),
  };

  for (double &i : main_grid) {
    i = dist(mt);
  }

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      int color = static_cast<int>((main_grid[i + j * width] + 1) / 2 * 255);
      image.setPixel(i, j, qRgb(color, color, color));
    }
  }
}

int main(int argc, char *argv[]) {

  std::vector<double> main_grid;
  std::vector<Color> colors;
  std::vector<Scale> scales;
  QImage image;
  RenderConfig rc(1, 500, 500);

  QApplication app(argc, argv);

  auto window = new QWidget;
  window->setWindowTitle("Multi-scale Turing pattern");
  window->setWindowIcon(QIcon(":/icon.png"));

  init(rc.width, rc.height, image, main_grid, colors, scales);

  auto renderconfig_layout = new QVBoxLayout;

  auto width_input = new QSpinBox;
  width_input->setMaximum(10000);
  width_input->setValue(rc.width);

  auto height_input = new QSpinBox;
  height_input->setMaximum(10000);
  height_input->setValue(rc.height);

  auto size_label = new QLabel("Size:");

  auto pass_input = new QSpinBox;
  pass_input->setMaximum(1000);
  pass_input->setValue(1);

  auto pass_label = new QLabel("Passes:");

  auto save_button = new QPushButton("Save");
  auto reset_button = new QPushButton("Reset");
  auto render_button = new QPushButton("Render");
  auto image_zone = new QLabel;

  auto pass_layout = new QHBoxLayout;
  auto size_layout = new QHBoxLayout;
  auto save_layout = new QHBoxLayout;
  auto main_layout = new QHBoxLayout;
  auto config_layout = new QVBoxLayout;

  auto scale_tabs = new QTabWidget;
  scale_tabs->setFixedWidth(400);
  //scale_tabs->setFixedHeight(200);

  for (int i = 0; i < scales.size(); ++i) {
    scale_tabs->addTab(new ScaleConfig(scales[i]), "Scale " + QString::number(i + 1));
  }

  config_layout->addWidget(scale_tabs);

  image_zone->setPixmap(QPixmap::fromImage(image));

  pass_layout->addWidget(pass_label);
  pass_layout->addWidget(pass_input);
  renderconfig_layout->addLayout(pass_layout);

  size_layout->addWidget(size_label);
  size_layout->addWidget(width_input);
  size_layout->addWidget(height_input);
  renderconfig_layout->addLayout(size_layout);

  save_layout->addWidget(save_button);
  renderconfig_layout->addLayout(save_layout);
  renderconfig_layout->addWidget(reset_button);
  renderconfig_layout->addWidget(render_button);
  config_layout->addLayout(renderconfig_layout);
  main_layout->addLayout(config_layout);
  main_layout->addWidget(image_zone);
  window->setLayout(main_layout);

  // Connect
  window->connect(render_button, &QPushButton::pressed, [&]{
      int input_w = width_input->value();
      int input_h = height_input->value();
      int input_p = pass_input->value();

      if (input_w != rc.width || input_h != rc.height) {
        rc.width = input_w;
        rc.height = input_h;

        init(rc.width, rc.height, image, main_grid, colors, scales);
      }

      if (input_p != rc.passes) {
        rc.passes = input_p;
      }

      generate(rc, main_grid, colors, scales, image);
      image_zone->setPixmap(QPixmap::fromImage(image.scaled(std::min(rc.width, 1000), std::min(rc.height, 1000), Qt::KeepAspectRatio)));
  });

  window->connect(reset_button, &QPushButton::pressed, [&]{
      init(rc.width, rc.height, image, main_grid, colors, scales);
      image_zone->setPixmap(QPixmap::fromImage(image.scaled(std::min(rc.width, 1000), std::min(rc.height, 1000), Qt::KeepAspectRatio)));
  });

  window->connect(save_button, &QPushButton::pressed, [&window, &image]{
      QString file = QFileDialog::getSaveFileName(window, "Save image", ".", "PNG file (*.png)");
      image.save(file, "png");
  });

  window->show();

  return app.exec();
}

