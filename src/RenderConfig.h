//
// Created by leogout on 07/12/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_RENDERCONFIG_H
#define MULTISCALE_TURING_PATTERN_CPP_RENDERCONFIG_H


class RenderConfig {
public:
    RenderConfig(int passes, int width, int height): passes(passes), width(width), height(height) {};
    int passes;
    int width;
    int height;
};


#endif //MULTISCALE_TURING_PATTERN_CPP_RENDERCONFIG_H
