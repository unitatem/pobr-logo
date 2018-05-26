//
// Created by mariusz on 24.05.18.
//

#include "DetectedObject.h"

#include <iomanip>

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

bool DetectedObject::check_for_S() {
    auto M1 = calculate_M1();
    auto M2 = calculate_M2();
    auto M4 = calculate_M4();
    return M1 > 0.26 && M1 < 0.35
            && M2 > 1e22
            && M4 > 1e18;
}

bool DetectedObject::check_for_Y() {
    auto M1 = calculate_M1();
    auto M2 = calculate_M2();
    auto M4 = calculate_M4();
    auto M7 = calculate_M7();
    return M1 > 0.30 && M1 < 0.50
           && M2 > 1e22
           && M4 > 1e17
           && M7 < -1e21;
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

double DetectedObject::calculate_M2() const {
    auto m00 = inertia_moment(0, 0);

    auto m20 = inertia_moment(2, 0);
    auto m10 = inertia_moment(1, 0);
    auto M20 = m20 - m10 * m10 / m00;

    auto m02 = inertia_moment(0, 2);
    auto m01 = inertia_moment(0, 1);
    auto M02 = m02 - m01 * m01 / m00;

    auto m11 = inertia_moment(1, 1);
    auto M11 = m11 - m10 * m01 / m00;

    return (pow(M20 - M02, 2.0) + 4.0 * M11 * M11) / pow(m00, 4.0);
}

double DetectedObject::calculate_M3() const {
    auto m00 = inertia_moment(0, 0);

    auto m30 = inertia_moment(3, 0);
    auto m20 = inertia_moment(2, 0);
    auto m10 = inertia_moment(1, 0);
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;

    auto m12 = inertia_moment(1, 2);
    auto m01 = inertia_moment(0, 1);
    auto y_dash = m01 / m00;
    auto m11 = inertia_moment(1, 1);
    auto m02 = inertia_moment(0, 2);
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;

    auto m21 = inertia_moment(2, 1);
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;

    auto m03 = inertia_moment(0, 3);
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    return (pow(M30 - 3 * M12, 2.0) + pow(3 * M21 - M03, 2.0)) / pow(m00, 5.0);
}

double DetectedObject::calculate_M4() const {
    auto m00 = inertia_moment(0, 0);

    auto m30 = inertia_moment(3, 0);
    auto m20 = inertia_moment(2, 0);
    auto m10 = inertia_moment(1, 0);
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;

    auto m12 = inertia_moment(1, 2);
    auto m01 = inertia_moment(0, 1);
    auto y_dash = m01 / m00;
    auto m11 = inertia_moment(1, 1);
    auto m02 = inertia_moment(0, 2);
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;

    auto m21 = inertia_moment(2, 1);
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;

    auto m03 = inertia_moment(0, 3);
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    return (pow(M30 + M12, 2.0) + pow(M21 + M03, 2.0)) / pow(m00, 5.0);
}

double DetectedObject::calculate_M5() const {
    auto m00 = inertia_moment(0, 0);

    auto m30 = inertia_moment(3, 0);
    auto m20 = inertia_moment(2, 0);
    auto m10 = inertia_moment(1, 0);
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;

    auto m12 = inertia_moment(1, 2);
    auto m01 = inertia_moment(0, 1);
    auto y_dash = m01 / m00;
    auto m11 = inertia_moment(1, 1);
    auto m02 = inertia_moment(0, 2);
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;

    auto m21 = inertia_moment(2, 1);
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;

    auto m03 = inertia_moment(0, 3);
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    return ((M30 - 3.0 * M12) * (M30 + M12) * (pow(M30 + M12, 2.0) - 3.0 * pow(M21 + M03, 2.0))
            + (3.0 * M21 - M03) * (M21 + M03) * (3 * pow(M30 + M12, 2.0) - pow(M21 + M03, 2.0))) / pow(m00, 10.0);
}

double DetectedObject::calculate_M6() const {
    auto m00 = inertia_moment(0, 0);

    auto m30 = inertia_moment(3, 0);
    auto m20 = inertia_moment(2, 0);
    auto m10 = inertia_moment(1, 0);
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;

    auto m12 = inertia_moment(1, 2);
    auto m01 = inertia_moment(0, 1);
    auto y_dash = m01 / m00;
    auto m11 = inertia_moment(1, 1);
    auto m02 = inertia_moment(0, 2);
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;

    auto m21 = inertia_moment(2, 1);
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;

    auto m03 = inertia_moment(0, 3);
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    auto M20 = m20 - x_dash * m10;
    auto M02 = m02 - y_dash * m01;

    auto M11 = m11 - x_dash * m01;

    return ((M20 - M02) * (pow(M30 + M12, 2.0) - pow(M21 + M03, 2.0))
            + 4.0 * M11 * (M30 + M12) * (M21 + M03)) / pow(m00, 7.0);
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
    os << "image:"
       << " AR=" << std::setw(12) << static_cast<double>(object.get_area()) / (object.image.cols * object.image.rows)
       << " M1=" << std::setw(9) << object.calculate_M1()
       << " M2=" << std::setw(10) << object.calculate_M2()
       << " M3=" << std::setw(12) << object.calculate_M3()
       << " M4=" << std::setw(12) << object.calculate_M4()
       << " M5=" << std::setw(13) << object.calculate_M5()
       << " M6=" << std::setw(12) << object.calculate_M6()
       << " M7=" << std::setw(13) << object.calculate_M7()
       << std::endl;
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
