//
// Created by mariusz on 24.05.18.
//

#include "DetectedObject.h"

DetectedObject::DetectedObject(const cv::Mat &image)
    : image(image)
{}

cv::Mat DetectedObject::get_image() const {
    return image;
}
