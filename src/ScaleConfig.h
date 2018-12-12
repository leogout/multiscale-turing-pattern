//
// Created by leogout on 16/11/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_SCALE_CONFIG_H
#define MULTISCALE_TURING_PATTERN_CPP_SCALE_CONFIG_H
#include <vector>

class ScaleConfig {
public:
    int aR;
    int iR;
    double sa;
    QColor color;

    ScaleConfig() = default;
    ScaleConfig(int aR, int iR, double sa, QColor color):
        aR(aR),
        iR(iR),
        sa(sa),
        color(color)
    {};
};


#endif //MULTISCALE_TURING_PATTERN_CPP_SCALE_CONFIG_H
