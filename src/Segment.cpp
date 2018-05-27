//
// Created by mariusz on 24.05.18.
//

#include "logger.h"
#include "Segment.h"
#include "Utils.h"


std::vector<DetectedObject> Segment::segment(cv::Mat &image) {
    assert(image.channels() == 1);

    std::vector<DetectedObject> result;
    for (auto r = 0u; r < image.rows; ++r)
        for (auto c = 0u; c < image.cols; ++c)
            if (image.at<uchar>(r, c) == 255) {
                auto segment = find_one(image, r, c);
                auto candidate = DetectedObject(segment);
                if (check_geometry_constraints(candidate))
                    result.push_back(candidate);
            }

    return result;
}

std::vector<DetectedObject> &Segment::filter_for_S(std::vector<DetectedObject> &detected) {
    for (auto it = detected.begin(); it != detected.end();) {
        if (it->check_for_S())
            ++it;
        else
            it = detected.erase(it);
    }

    return detected;
}

std::vector<DetectedObject> &Segment::filter_for_Y(std::vector<DetectedObject> &detected) {
    for (auto it = detected.begin(); it != detected.end();) {
        if (it->check_for_Y())
            ++it;
        else
            it = detected.erase(it);
    }

    return detected;
}

std::vector<DetectedObject>
Segment::find_best_pair(const std::vector<DetectedObject> &ss, const std::vector<DetectedObject> &yy) {
    if (ss.size() == 0) {
        std::cout << "WARNING: No S object to merge" << std::endl;
        return yy;
    }

    if (yy.size() == 0) {
        std::cout << "WARNING: No Y object to merge" << std::endl;
        return ss;
    }

    auto best_ss = 0;
    auto best_yy = 0;
    auto best_orientation = M_PI;
    for (auto idx_ss = 0; idx_ss < ss.size(); ++idx_ss) {
        auto center_ss = ss[idx_ss].get_center();
        for (auto idx_yy = 0; idx_yy < yy.size(); ++idx_yy) {
            auto center_yy = yy[idx_yy].get_center();
            auto diff = center_yy - center_ss;

            auto orientation = atan2(diff.y, diff.x);
            if (abs(orientation) < abs(best_orientation)) {
                best_orientation = orientation;
                best_ss = idx_ss;
                best_yy = idx_yy;
            }
        }
    }

    std::vector<DetectedObject> result;
    result.push_back(ss[best_ss]);
    result.push_back(yy[best_yy]);
    return result;
}

bool Segment::check_geometry_constraints(const DetectedObject &object) {
    return object.get_area() / (object.get_image().rows * object.get_image().cols) >
           0.0009; // && object.get_area() < object.get_image().rows * object.get_image().cols * 0.05;
}

cv::Mat Segment::find_one(cv::Mat &image, int row, int col) {
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
