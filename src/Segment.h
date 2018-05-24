//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_SEGMENT_H
#define POBR_LOGO_SEGMENT_H

#include "DetectedObject.h"

#include "opencv2/core/core.hpp"

#include <vector>

class Segment {
public:
    static cv::Mat to_mono(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high);
    static cv::Mat to_rgb(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high);

private:
    static bool op_ge(const cv::Vec3b &value, const cv::Vec3b &ref);
    static bool op_le(const cv::Vec3b &value, const cv::Vec3b &ref);
};


#endif //POBR_LOGO_SEGMENT_H
