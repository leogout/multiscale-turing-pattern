//
// Created by leogout on 16/11/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_COLOR_H
#define MULTISCALE_TURING_PATTERN_CPP_COLOR_H


#include <ostream>

struct Color {
    typedef unsigned char byte;
    byte red, green, blue;
    Color():red(0), green(0), blue(0){};
    Color(byte r, byte g, byte b):red(r), green(g), blue(b){};

    void bump_to(Color to, double amount) {
      red = static_cast<byte>((red * (1 - amount)) + (to.red * amount));
      green = static_cast<byte>((green * (1 - amount)) + (to.green * amount));
      blue = static_cast<byte>((blue * (1 - amount)) + (to.blue * amount));
    }

    Color operator* (double x) {
      return {
          static_cast<byte>(red * x),
          static_cast<byte>(green * x),
          static_cast<byte>(blue * x)
      };
    }

    friend std::ostream& operator<<(std::ostream& os, const Color &obj) {
      os << obj.red
         << obj.green
         << obj.blue;

      return os;
    }
};


#endif //MULTISCALE_TURING_PATTERN_CPP_COLOR_H
