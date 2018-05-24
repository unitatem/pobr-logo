//
// Created by mariusz on 24.05.18.
//

#include "Detector.h"
#include "Utils.h"


std::vector<DetectedObject> Detector::detect(cv::Mat &image) {
    assert(image.channels() == 1);

    std::vector<DetectedObject> result;
    for (auto r = 0u; r < image.rows; ++r)
        for (auto c = 0u; c < image.cols; ++c)
            if (image.at<uchar>(r, c) == 255)
                result.emplace_back(detect_one(image, r, c));

    return result;
}


cv::Mat Detector::detect_one(cv::Mat &image, int row, int col) {
    cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    std::vector<cv::Point> to_visit;
    to_visit.emplace_back(row, col);
    while (!to_visit.empty()) {
        auto point = to_visit.back();
        to_visit.pop_back();
        result.at<uchar>(point.x, point.y) = 255;
        image.at<uchar>(point.x, point.y) = 0;

        for (auto r = -1; r < 2; ++r)
            for (auto c = -1; c < 2; ++c) {
                if (point.x + r < 0 || point.x + r >= image.rows || point.y + c < 0 || point.y + c >= image.cols)
                    continue;
                if (image.at<uchar>(point.x + r, point.y + c) == 255)
                    to_visit.emplace_back(point.x + r, point.y + c);
            }
    }

    return result;
}

