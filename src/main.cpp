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
#include <src/view/ScalesConfig.h>

#include "Scale.h"
#include "src/view/ScaleConfigWidget.h"
#include "RenderConfig.h"
#include "src/view/RenderConfigWidget.h"
#include "ScaleConfig.h"

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

void multiply(QColor &color, double factor) {
  color.setRgb(
      static_cast<int>(color.red() * factor),
      static_cast<int>(color.green() * factor),
      static_cast<int>(color.blue() * factor)
  );
}

void bump_to(QColor &from, QColor &to, double amount) {
  from.setRgb(
      static_cast<int>(from.red() * (1 - amount) + to.red() * amount),
      static_cast<int>(from.green() * (1 - amount) + to.green() * amount),
      static_cast<int>(from.blue() * (1 - amount) + to.blue() * amount)
  );
}

void generate(RenderConfig &rc, std::vector<double> &main_grid, std::vector<QColor> &colors, std::vector<ScaleConfig> scalesConfig, std::vector<Scale> &scales, QImage &image) {
  int width = rc.width;
  int height = rc.height;
  std::vector<double> buffer(width * height);

  for (int pass = 0; pass < rc.passes; ++pass) {
    std::cout << "Pass " << pass << std::endl;
    for (int scale = 0; scale < scales.size(); ++scale) {
      blur(main_grid, scales[scale].activator, buffer, scalesConfig[scale].aR, width, height);
      blur(main_grid, scales[scale].inhibitor, buffer, scalesConfig[scale].iR, width, height);

      for (int i = 0; i < main_grid.size(); ++i) {
        scales[scale].variation[i] = fabs(
            scales[scale].activator[i] - scales[scale].inhibitor[i]
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
        factor = scalesConfig[best_scale].sa;
      } else {
        factor = -scalesConfig[best_scale].sa;
      }

      bump_to(colors[i], scalesConfig[best_scale].color, 0.3);
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
      int idx = i + j * width;
      multiply(colors[idx], (main_grid[idx] + 1) / 2);
      image.setPixel(i, j, colors[idx].rgb());
    }
  }
}

void init(RenderConfig rc, QImage &image, std::vector<double> &main_grid, std::vector<QColor> &colors, std::vector<Scale> &scales) {
  std::random_device rd;
  std::mt19937 mt(5621);
  std::uniform_real_distribution<double> dist(-1, 1);
  int width = rc.width;
  int height = rc.height;
  unsigned long size = static_cast<unsigned long>(width * height);

  main_grid = std::vector<double>(width * height);
  colors = std::vector<QColor>(width * height);
  scales = std::vector<Scale>(5);
  image = QImage(width, height, QImage::Format_RGB888);

  for (int i = 0; i < 5; ++i) {
    scales[i] = Scale(size);
  }

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
  std::vector<QColor> colors;
  QImage image;
  RenderConfig rc(1, 500, 500);
  std::vector<Scale> scales;
  std::vector<ScaleConfig> scalesConfig;

  scalesConfig = {
      ScaleConfig(100, 200, 0.05, QColor(237, 255, 143)),
      ScaleConfig(20, 40, 0.04, QColor(232, 204, 81)),
      ScaleConfig(10, 20, 0.03, QColor(255, 198, 116)),
      ScaleConfig(5, 10, 0.02, QColor(232, 133, 98)),
      ScaleConfig(1, 2, 0.01, QColor(255, 131, 162)),
  };

  init(rc, image, main_grid, colors, scales);

  QApplication app(argc, argv);

  // Main Window
  auto window = new QWidget;
  window->setWindowTitle("Multi-scale Turing pattern");
  window->setWindowIcon(QIcon(":/icon.png"));

  // Buttons
  auto save_button = new QPushButton("Save");
  auto render_button = new QPushButton("Render");

  // Image
  auto image_zone = new QLabel;
  image_zone->setPixmap(QPixmap::fromImage(image));

  // Custom widgets
  auto render_config = new RenderConfigWidget(rc);
  auto scales_config = new ScalesConfig(scalesConfig);

  // Layouts
  auto main_layout = new QHBoxLayout;
  auto config_layout = new QVBoxLayout;

  config_layout->addWidget(scales_config);
  config_layout->addWidget(render_config);
  config_layout->addWidget(render_button);
  config_layout->addWidget(save_button);

  main_layout->addLayout(config_layout);
  main_layout->addWidget(image_zone);

  window->setLayout(main_layout);

  // Connect
  window->connect(render_button, &QPushButton::pressed, [&]{
      init(rc, image, main_grid, colors, scales);
      generate(rc, main_grid, colors, scales_config->getConfig(), scales, image);
      image_zone->setPixmap(QPixmap::fromImage(image.scaled(std::min(rc.width, 1000), std::min(rc.height, 1000), Qt::KeepAspectRatio)));
  });

  window->connect(save_button, &QPushButton::pressed, [&window, &image]{
      QString file = QFileDialog::getSaveFileName(window, "Save image", ".", "PNG file (*.png)");
      image.save(file, "png");
  });

  window->show();

  return app.exec();
}

