//
// Created by mariusz on 24.05.18.
//

#include "DetectedObject.h"

DetectedObject::DetectedObject(const cv::Mat &image)
        : image(image) {}

cv::Mat DetectedObject::get_image() const {
    return image;
}

unsigned long long int DetectedObject::inertia_moment(int vertical_order, int horizontal_order) const {
    assert(image.channels() == 1);

    unsigned long long int value = 0;
    for (auto r = 0u; r < image.rows; ++r)
        for (auto c = 0u; c < image.cols; ++c)
            if (image.at<uchar>(r, c) == 255)
                value += pow(r, vertical_order) * pow(c, horizontal_order);

    return value;
}

unsigned long long int DetectedObject::get_area() const {
    return inertia_moment(0, 0);
}

double DetectedObject::calculate_M1() const {
    auto m00 = inertia_moment(0, 0);

    auto m20 = inertia_moment(2, 0);
    auto m10 = inertia_moment(1, 0);
    auto M20 = m20 - pow(m10, 2.0) / m00;

    auto m02 = inertia_moment(0, 2);
    auto m01 = inertia_moment(0, 1);
    auto M02 = m02 - pow(m01, 2.0) / m00;

    return (M20 + M02) / pow(m00, 2.0);
}

double DetectedObject::calculate_M7() const {
    auto m00 = inertia_moment(0, 0);
    auto m10 = inertia_moment(1, 0);
    auto m01 = inertia_moment(0, 1);

    auto m20 = inertia_moment(2, 0);
    auto M20 = m20 - pow(m10, 2.0) / m00;

    auto m02 = inertia_moment(0, 2);
    auto M02 = m02 - pow(m01, 2.0) / m00;

    auto m11 = inertia_moment(1, 1);
    auto M11 = m11 - m10 * m01 / m00;

    auto result = (M20 * M02 - pow(M11, 2.0)) / pow(m00, 4.0);
    return result;
}

std::ostream &operator<<(std::ostream &os, const DetectedObject &object) {
    os << "image: " << object.get_area() << " " << object.calculate_M1() << " " << object.calculate_M7() << std::endl;
    return os;
}

cv::Rect DetectedObject::find_bounding_box() {
    assert(image.channels() == 1);

    auto left = std::numeric_limits<int>::max();
    auto right = std::numeric_limits<int>::min();
    auto up = std::numeric_limits<int>::max();
    auto down = std::numeric_limits<int>::min();

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c)
            if (image.at<uchar>(r, c) == 255) {
                if (c < left)
                    left = c;
                if (c > right)
                    right = c;
                if (r < up)
                    up = r;
                if (r > down)
                    down = r;
            }
    return {left, up, right - left, down - up};
}

