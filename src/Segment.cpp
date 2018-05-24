//
// Created by mariusz on 24.05.18.
//

#include "Segment.h"

cv::Mat Segment::to_mono(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            const auto &pixel = image.at<cv::Vec3b>(r, c);
            if (op_ge(pixel, low) && op_le(pixel, high))
                result.at<uchar >(r, c) = 255;
        }

    return result;
}

cv::Mat Segment::to_rgb(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC3, cv::Vec3b(0, 0, 0));

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            const auto &pixel = image.at<cv::Vec3b>(r, c);
            if (op_ge(pixel, low) && op_le(pixel, high))
                result.at<cv::Vec3b>(r, c) = pixel;
        }

    return result;
}

bool Segment::op_ge(const cv::Vec3b &value, const cv::Vec3b &ref) {
    return value[0] >= ref[0] && value[1] >= ref[1] && value[2] >= ref[2];
}

bool Segment::op_le(const cv::Vec3b &value, const cv::Vec3b &ref) {
    return value[0] <= ref[0] && value[1] <= ref[1] && value[2] <= ref[2];
}


