#ifndef _COLOR_HPP
#define _COLOR_HPP

#include <stdint.h>

#include "helper.hpp"
#include "scene_parser.hpp"

enum ColorChannel {
    CC_RED,
    CC_GREEN,
    CC_BLUE,
    CC_ALPHA,
    CC_N
};

class Color {
public:
    Color(double r = 0.0,
          double g = 0.0,
          double b = 0.0,
          double a = 1.0);
    Color(SceneComponent* sc, uint16_t start = 0);

    void SetChannel(int channel, double value);

    /* Export to raw 24-bit color */
    void Output8BitPixel(uint8_t* dst) const;

    Color Inverted() const;

    double Luminance() const;

    /* Component-wise scalar multiplication */
    Color operator* (double d) const;
    Color operator* (const Color& c) const;

    /* addition */
    Color operator+  (const Color& col) const;
    void operator+= (const Color& col);

    static Color Average(const std::vector<Color>& colors);

private:
    /* Each channel is clamped between 0.0 and 1.0, inclusive. */
    double channels[CC_N];

    inline void clamp_channels() {
        for (int i = 0; i < CC_N; i++) {
            channels[i] = clamp(channels[i]);
        }
    }
};

#endif
