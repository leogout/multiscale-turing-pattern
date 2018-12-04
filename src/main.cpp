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
#include "StepConfig.h"

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

void generate(int width, int height, std::vector<double> &main_grid, std::vector<Color> &colors, std::vector<Scale> &scales, QImage &image) {
  std::vector<double> buffer(width * height);

  //for (int passes = 0; passes < PASSES; ++passes) {
  //  std::cout << "Pass " << passes << std::endl;
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
  //}

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
      Scale(width * height, 200, 50, 0.05, Color(0, 150, 20)),
      Scale(width * height, 100, 25, 0.04, Color(40, 180, 70)),
      Scale(width * height, 50, 10, 0.03, Color(150, 200, 30)),
      Scale(width * height, 25, 5, 0.02, Color(100, 100, 255)),
      Scale(width * height, 10, 2, 0.01, Color(200, 50, 50)),
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
  int WIDTH = 500;
  int HEIGHT = 500;
  std::vector<double> main_grid;
  std::vector<Color> colors;
  std::vector<Scale> scales;
  QImage image;

  QApplication app(argc, argv);

  QWidget *window = new QWidget;

  init(WIDTH, HEIGHT, image, main_grid, colors, scales);

  auto width_input = new QSpinBox;
  width_input->setMaximum(1920);
  width_input->setValue(WIDTH);

  auto height_input = new QSpinBox;
  height_input->setMaximum(1080);
  height_input->setValue(HEIGHT);

  auto size_button = new QPushButton("Update");

  auto save_input = new QLineEdit("../out/filename.png");
  auto save_button = new QPushButton("Save");
  auto render_button = new QPushButton("Render");
  auto image_zone = new QLabel;

  auto size_layout = new QHBoxLayout;
  auto save_layout = new QHBoxLayout;
  auto main_layout = new QHBoxLayout;
  auto config_layout = new QVBoxLayout;

  auto scale_tabs = new QTabWidget;
  scale_tabs->setFixedWidth(400);
  scale_tabs->setFixedHeight(200);

  int i = 1;
  for (auto &scale: scales) {
    scale_tabs->addTab(new StepConfig(scale), "Scale " + QString::number(i));
    i++;
  }
  config_layout->addWidget(scale_tabs);

  image_zone->setPixmap(QPixmap::fromImage(image));

  size_layout->addWidget(width_input);
  size_layout->addWidget(height_input);
  size_layout->addWidget(size_button);
  config_layout->addLayout(size_layout);

  save_layout->addWidget(save_input);
  save_layout->addWidget(save_button);
  config_layout->addLayout(save_layout);
  config_layout->addWidget(render_button);
  main_layout->addLayout(config_layout);
  main_layout->addWidget(image_zone);
  window->setLayout(main_layout);

  // Connect
  window->connect(render_button, &QPushButton::pressed, [WIDTH, HEIGHT, &width_input, &height_input, &main_grid, &colors, &scales, &image, &image_zone]{
      generate(width_input->value(), height_input->value(), main_grid, colors, scales, image);

      image_zone->setPixmap(QPixmap::fromImage(image.scaled(WIDTH, HEIGHT, Qt::KeepAspectRatio)));
  });

  window->connect(save_button, &QPushButton::pressed, [&image, &save_input]{
      image.save(save_input->text(), "png");
  });

  window->connect(size_button, &QPushButton::pressed, [&main_grid, &colors, &image, &scales, &width_input, &height_input]{
      init(width_input->value(), height_input->value(), image, main_grid, colors, scales);
  });

  window->show();

  return app.exec();
}

