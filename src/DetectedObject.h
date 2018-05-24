//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_DETECTEDOBJECT_H
#define POBR_LOGO_DETECTEDOBJECT_H

#include "opencv2/core/core.hpp"

class DetectedObject {
public:
    DetectedObject(const cv::Mat &image);
    cv::Mat get_image() const;

private:
    const cv::Mat image;
};


#endif //POBR_LOGO_DETECTEDOBJECT_H
