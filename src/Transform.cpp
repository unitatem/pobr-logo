//
// Created by mariusz on 24.05.18.
//

#include "Transform.h"


cv::Mat Transform::channels2gray(const cv::Mat &image, const cv::Vec3b &factor) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            const auto &pixel = image.at<cv::Vec3b>(r, c);
            auto val = pixel[0] * factor[0] + pixel[1] * factor[1] + pixel[2] * factor[2];
            result.at<uchar>(r, c) = val;
        }

    return result;
}

cv::Mat Transform::channels2black(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    cv::Vec3i low_adj = low;
    cv::Vec3i high_adj = high;
    auto inverse_0 = low[0] > high[0];
    if (inverse_0)
        high_adj[0] += 255;
    auto inverse_1 = low[1] > high[1];
    if (inverse_1)
        high_adj[1] += 255;
    auto inverse_2 = low[2] > high[2];
    if (inverse_2)
        high_adj[2] += 255;

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            cv::Vec3i pixel = image.at<cv::Vec3b>(r, c);
            if (inverse_0 && pixel[0] < high[0])
                pixel[0] += 255;
            if (inverse_1 && pixel[1] < high[1])
                pixel[1] += 255;
            if (inverse_2 && pixel[2] < high[2])
                pixel[2] += 255;

            if (op_ge(pixel, low_adj) && op_le(pixel, high_adj))
                result.at<uchar>(r, c) = 255;
        }

    return result;
}

cv::Mat Transform::rgb2rgb(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high) {
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

cv::Mat Transform::rgb2hsv(const cv::Mat &image) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC3, cv::Vec3b(0, 0, 0));

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            const auto &pixel = image.at<cv::Vec3b>(r, c);
            auto red = pixel[0];
            auto green = pixel[1];
            auto blue = pixel[2];

            auto c_max = std::max(red, std::max(green, blue));
            auto c_min = std::min(red, std::min(green, blue));
            auto delta = c_max - c_min;

            uchar hue = 0;
            if (delta == 0)
                hue = 0;
            else if (c_max == red)
                hue = static_cast<int>((0.0 + static_cast<double>(green - blue) / delta) * 42.5);
            else if (c_max == green)
                hue = static_cast<int>((2.0 + static_cast<double>(blue - red) / delta) * 42.5);
            else
                hue = static_cast<int>((4.0 + static_cast<double>(red - green) / delta) * 42.5);

            uchar saturation = 0;
            if (c_max != 0)
                saturation = static_cast<double>(delta) / c_max * 255;

            auto value = c_max;

            result.at<cv::Vec3b>(r, c) = cv::Vec3b(hue, saturation, value);
        }

    return result;
}

bool Transform::op_ge(const cv::Vec3i &value, const cv::Vec3i &ref) {
    return value[0] >= ref[0] && value[1] >= ref[1] && value[2] >= ref[2];
}

bool Transform::op_le(const cv::Vec3i &value, const cv::Vec3i &ref) {
    return value[0] <= ref[0] && value[1] <= ref[1] && value[2] <= ref[2];
}
