//
// Created by leogout on 16/11/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_SCALE_H
#define MULTISCALE_TURING_PATTERN_CPP_SCALE_H
#include <vector>
#include "Color.h"


class ScaleConfig {
public:
    std::vector<double> activator;
    std::vector<double> inhibitor;
    std::vector<double> variation;
    int aR;
    int iR;
    double sa;
    QColor color;

    ScaleConfig() = default;
    ScaleConfig(unsigned long size, int aR, int iR, double sa, QColor color):
        aR(aR),
        iR(iR),
        sa(sa),
        activator(size),
        inhibitor(size),
        variation(size),
        color(color)
    {};
};


#endif //MULTISCALE_TURING_PATTERN_CPP_SCALE_H
