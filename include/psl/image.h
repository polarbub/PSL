#pragma once
#ifndef psl_image_h
#define psl_image_h

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

namespace psl {
    //My image system
//FIX: Make this use an array not a vector by overriding the [] operator
    class Image : public std::vector<std::vector<psl::image::Color>> {
    private:
        std::string path;
    public:
        //This can be really slow, just wait.
        void writeFile(std::string filename);
        void writeFile();
        int loadFile(std::string filename);

        Image(std::string fileName);
        Image();

        int getHeight();
        int getWidth();
    };
}

#endif