#pragma once

#include "psl.h"

namespace psl::image {
    struct Color {
        uchar r;
        uchar g;
        uchar b;

        bool operator!=(const Color& c);

        bool operator==(const Color& c);
    };
#define Pixel Color
}