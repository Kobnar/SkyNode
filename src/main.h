// main.h
#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>

#include "lib/inih/ini.c"
#include "lib/inih/cpp/INIReader.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

namespace SkyNode
{
    bool DEBUG_MODE;
    bool TRACKING_MODE;
    long FRAME_BLUR;
    long THRESHOLD_SENSITIVITY;
    long THRESHOLD_ERODE;
    long THRESHOLD_DIALATE;
}

int main();

#endif // MAIN_H_