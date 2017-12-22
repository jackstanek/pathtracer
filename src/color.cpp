#include <cassert>
#include "stdint.h"

#include "color.hpp"
#include "helper.hpp"

Color::Color(double r, double g, double b, double a)
{
    this->channels[CC_RED] = r;
    this->channels[CC_GREEN] = g;
    this->channels[CC_BLUE] = b;
    this->channels[CC_ALPHA] = a;
}

Color::Color(SceneComponent* sc, uint16_t start)
{
    for (uint16_t i = start; i < start + CC_ALPHA; i++) {
        this->channels[i] = sc->values()[i].d_val;
    }
}

void Color::SetChannel(int channel, double value)
{
    assert(channel >= 0 && channel < CC_N);
    this->channels[channel] = value;
}
void Color::Output8BitPixel(uint8_t* dst) const
{
    for (int i = 0; i < CC_N; i++) {
        dst[i] = 255 * clamp(this->channels[i]);
    }
}

Color Color::Inverted() const
{
    return Color(1.0 - this->channels[CC_RED],
                 1.0 - this->channels[CC_GREEN],
                 1.0 - this->channels[CC_BLUE],
                 this->channels[CC_ALPHA]);
}

double Color::Luminance() const
{
    return 0.299 * this->channels[CC_RED]
        +  0.587 * this->channels[CC_GREEN]
        +  0.114 * this->channels[CC_BLUE];
}

Color Color::operator* (double d) const
{
    return Color(this->channels[CC_RED] * d,
                 this->channels[CC_GREEN] * d,
                 this->channels[CC_BLUE] * d,
                 this->channels[CC_ALPHA]);
}

Color Color::operator* (const Color& col) const
{
    double mults[CC_N];
    for (int i = 0; i < CC_ALPHA; i++) {
        mults[i] = this->channels[i] * col.channels[i];
    }
    return Color(mults[0],
                 mults[1],
                 mults[2],
                 1);
}

Color Color::operator+ (const Color& col) const
{
    return Color(this->channels[CC_RED] + col.channels[CC_RED],
                 this->channels[CC_GREEN] + col.channels[CC_GREEN],
                 this->channels[CC_BLUE] + col.channels[CC_BLUE],
                 this->channels[CC_ALPHA] + col.channels[CC_ALPHA]);
}

void Color::operator+= (const Color& col)
{
    for (int i = 0; i < CC_N; i++) {
        this->channels[i] += col.channels[i];
    }
}

Color Color::Average(const std::vector<Color>& colors)
{
    double totals[CC_N] = {0, 0, 0, 0};

    uint32_t i;
    for (auto c = colors.begin(); c != colors.end(); c++) {
        for (i = 0; i < CC_N; i++) {
            totals[i] += c->channels[i];
        }
    }

    for (i = 0; i < CC_N; i++) {
        totals[i] /= colors.size();
    }

    return Color(totals[0], totals[1], totals[2], totals[3]);
}
