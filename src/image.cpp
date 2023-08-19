#include "psl.h"
#include "image.h"

#include "vector"
#include "iostream"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

//This can be really slow, just wait.
void psl::Image::writeFile(std::string filename) {
    int width = this->getWidth();
    int height = this->getHeight();
    uchar *stbImageArray = new uchar[width * height * 3];
    int i = 0;
    for (std::vector<psl::image::Pixel> vp: *this) {
        for (psl::image::Pixel pixel: vp) {
            stbImageArray[i] = pixel.r;

            i++;
            stbImageArray[i] = pixel.g;

            i++;
            stbImageArray[i] = pixel.b;

            i++;
        }
    }

    stbi_write_png(filename.c_str(), width, height, 3, stbImageArray, width * 3);

    delete[] stbImageArray;
}

void psl::Image::writeFile() {
    writeFile(this->path);
}

int psl::Image::loadFile(std::string filename) {
    this->path = filename;

    int width;
    int height;
    int bitsPerPixel;

    //See https://github.com/nothings/stb/blob/master/stb_image.h#L167
    unsigned char *imageIn = stbi_load(filename.c_str(), &width, &height, &bitsPerPixel, 3);
    if (imageIn == NULL) {
        return 1;
    }

    ullong i = 0;
    std::vector<psl::image::Pixel> row;
    ullong imageSize = width * height * 3;
    int bytesPerLine = width * 3;

    while (i < imageSize) {
        psl::image::Pixel pixel;
        pixel.r = imageIn[i];

        i++;
        pixel.g = imageIn[i];

        i++;
        pixel.b = imageIn[i];

        row.push_back(pixel);

        //Check if a row is done and then push it back into the image.
        if ((i + 1) % bytesPerLine == 0 && i != 0) {
            this->push_back(row);
            row.clear();
        }
        i++;
    }

    return 0;
}

psl::Image::Image(std::string fileName) {
    loadFile(fileName);
}

psl::Image::Image() {};

int psl::Image::getHeight() {
    return this->size();
}

int psl::Image::getWidth() {
    if (this->size() > 0) {
        return this->at(0).size();
    } else {
        return 0;
    }
}

bool psl::image::Color::operator!=(const Color& c) {
    return !((this->r == c.r) && (this->b == c.b) && (this->g == c.g));
}

bool psl::image::Color::operator==(const Color& c) {
    return (this->r == c.r) && (this->b == c.b) && (this->g == c.g);
}
