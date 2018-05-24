//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_DETECTOR_H
#define POBR_LOGO_DETECTOR_H

#include "DetectedObject.h"

#include "opencv2/core/core.hpp"

class Detector {
public:
    static std::vector<DetectedObject> detect(cv::Mat &image);

    static std::vector<DetectedObject> &filter(std::vector<DetectedObject> &detected);

private:
    static cv::Mat detect_one(cv::Mat &image, int row, int col);
};


#endif //POBR_LOGO_DETECTOR_H
