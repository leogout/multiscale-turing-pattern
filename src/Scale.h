//
// Created by leogout on 16/11/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_SCALE_H
#define MULTISCALE_TURING_PATTERN_CPP_SCALE_H
#include <vector>
#include <QtGui/QColor>


class Scale {
public:
    std::vector<double> activator;
    std::vector<double> inhibitor;
    std::vector<double> variation;

    Scale() = default;
    Scale(unsigned long size):
        activator(size),
        inhibitor(size),
        variation(size)
    {};
};


#endif //MULTISCALE_TURING_PATTERN_CPP_SCALE_H
