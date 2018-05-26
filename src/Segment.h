//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_SEGMENT_H
#define POBR_LOGO_SEGMENT_H

#include "DetectedObject.h"

#include "opencv2/core/core.hpp"

class Segment {
public:
    static std::vector<DetectedObject> segment(cv::Mat &image);

    static std::vector<DetectedObject> &filter_for_S(std::vector<DetectedObject> &detected);
    static std::vector<DetectedObject> &filter_for_Y(std::vector<DetectedObject> &detected);

private:
    static bool check_area_constraints(const DetectedObject &object);
    static cv::Mat find_one(cv::Mat &image, int row, int col);
};


#endif //POBR_LOGO_SEGMENT_H
