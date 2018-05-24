//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_MORPHOLOGY_H
#define POBR_LOGO_MORPHOLOGY_H

#include "ConvolutionMask.h"

#include "opencv2/core/core.hpp"

class Morphology {
public:
    static cv::Mat open(cv::Mat image);
    static cv::Mat close(cv::Mat image);

    static cv::Mat erosion(cv::Mat image);
    static cv::Mat dilation(cv::Mat image);

private:
    using IfSetPixelChecker = bool (*)(int value);
    static cv::Mat convolution(cv::Mat image, const ConvolutionMask &mask, IfSetPixelChecker valueChecker);
};


#endif //POBR_LOGO_MORPHOLOGY_H
