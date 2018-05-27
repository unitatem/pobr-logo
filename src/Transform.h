//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_TRANSFORM_H
#define POBR_LOGO_TRANSFORM_H

#include "DetectedObject.h"

#include "opencv2/core/core.hpp"

#include <vector>

class Transform {
public:
    static cv::Mat channels2gray(const cv::Mat &image, const cv::Vec3b &factor);
    static cv::Mat channels2black(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high);

    static cv::Mat rgb2rgb(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high);

    static cv::Mat rgb2hsv(const cv::Mat &image);

private:
    static bool op_ge(const cv::Vec3i &value, const cv::Vec3i &ref);
    static bool op_le(const cv::Vec3i &value, const cv::Vec3i &ref);
};


#endif //POBR_LOGO_TRANSFORM_H
