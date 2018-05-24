//
// Created by mariusz on 24.05.18.
//

#include "Morphology.h"

cv::Mat Morphology::open(cv::Mat image) {
    assert(image.channels() == 1);
    return dilation(erosion(image));
}

cv::Mat Morphology::close(cv::Mat image) {
    assert(image.channels() == 1);
    return erosion(dilation(image));
}

cv::Mat Morphology::erosion(cv::Mat image) {
    assert(image.channels() == 1);

    static const auto &mask = ConvolutionMask::ones;
    static auto fun = [](int value) {
        return value == 9 * 255;
    };

    return convolution(image, mask, fun);
}

cv::Mat Morphology::dilation(cv::Mat image) {
    assert(image.channels() == 1);

    static const auto &mask = ConvolutionMask::ones;
    static auto fun = [](int value) {
        return value != 0;
    };

    return convolution(image, mask, fun);
}

cv::Mat
Morphology::convolution(cv::Mat image, const ConvolutionMask &mask, Morphology::IfSetPixelChecker valueChecker) {
    cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));
    for (auto r = mask.getBorderRows(); r < image.rows - mask.getBorderRows(); ++r)
        for (auto c = mask.getBorderCols(); c < image.cols - mask.getBorderCols(); ++c) {
            auto value = mask.convolve(image, r, c);
            assert(value >= 0 && value % 255 == 0 && value <= 9 * 255);

            if (valueChecker(value))
                result.at<uchar>(r, c) = 255;
            else
                result.at<uchar>(r, c) = 0;
        }

    return cv::Mat(result);
}
