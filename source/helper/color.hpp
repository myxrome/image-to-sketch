//
// Created by Aleksey on 28.04.2015.
//

#ifndef PALETTE_DATA_GENERATOR_COLOR_H
#define PALETTE_DATA_GENERATOR_COLOR_H

#include <opencv2/core.hpp>

namespace color {

    using namespace cv;

    double calculateColorDistanceCIE76(Vec3f lab_color1, Vec3f lab_color2);

    double calculateColorDistanceCIEDE2000(Vec3f lab_color1, Vec3f lab_color2);

    int colorToIndex(int b, int g, int r);

    int colorToIndex(Vec3b color);

    Vec3f fixLabColor(Vec3b color);

    Mat createLabColorMat();

}

#endif //PALETTE_DATA_GENERATOR_COLOR_H
