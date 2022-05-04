#include "psl.h"
#include "image.h"
#include "vector"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

class psl::Image : public std::vector<std::vector<psl::image::Pixel>> {
public:
    //This can be really slow, just wait.
    void writeFile(std::string filename) {
        int width = this->getWidth();
        int height = this->getHeight();
        uchar* stbImageArray = new uchar[width * height * 3];
        int i = 0;
        for (std::vector<psl::image::Pixel> vp : *this) {
            for (psl::image::Pixel pixel : vp) {
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

    void loadFile(std::string filename) {
        int width;
        int height;
        int bitsPerPixel;

        //See https://github.com/nothings/stb/blob/master/stb_image.h#L167
        unsigned char* imageIn = stbi_load(filename.c_str(), &width , &height, &bitsPerPixel, 3);

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
    }

    Image(std::string fileName) {
        loadFile(fileName);
    }

    Image() {};

    int getHeight() {
        return this->size();
    }

    int getWidth() {
        if(this->size() > 0) {
            return this[0].size();
        } else {
            return 0;
        }
    }
};

/*class psl::Image {
public:
    std::vector<std::vector<psl::image::Pixel>> image;

    int getHeight() {
        return image.size();
    }

    int getWidth() {
        if(image.size() > 0) {
            return image[0].size();
        } else {
            return 0;
        }
    }
};*/