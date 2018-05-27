//
// Created by mariusz on 24.05.18.
//

#include "DetectedObject.h"
#include "logger.h"

#include <iomanip>

DetectedObject::DetectedObject(const cv::Mat &image)
        : image(image) {
    calculate_all_inertia_moment();
}

cv::Mat DetectedObject::get_image() const {
    return image;
}

double DetectedObject::inertia_moment(int vertical_order, int horizontal_order) const {
    assert(image.channels() == 1);

    auto value = 0.0;
    for (auto r = 0u; r < image.rows; ++r)
        for (auto c = 0u; c < image.cols; ++c)
            if (image.at<uchar>(r, c) == 255)
                value += pow(r, vertical_order) * pow(c, horizontal_order);

    return value;
}

void DetectedObject::calculate_all_inertia_moment() {
    assert(image.channels() == 1);

    m00 = 0.0;
    m10 = m20 = m30 = 0.0;
    m01 = m02 = m03 = 0.0;
    m21 = m11 = m12 = 0.0;

    for (auto r = 0u; r < image.rows; ++r)
        for (auto c = 0u; c < image.cols; ++c)
            if (image.at<uchar>(r, c) == 255) {
//                value += pow(r, vertical_order) * pow(c, horizontal_order);

                m00 += pow(r, 0.0) * pow(c, 0.0);

                m10 += pow(r, 1.0) * pow(c, 0.0);
                m20 += pow(r, 2.0) * pow(c, 0.0);
                m30 += pow(r, 3.0) * pow(c, 0.0);

                m01 += pow(r, 0.0) * pow(c, 1.0);
                m02 += pow(r, 0.0) * pow(c, 2.0);
                m03 += pow(r, 0.0) * pow(c, 3.0);

                m21 += pow(r, 2.0) * pow(c, 1.0);
                m11 += pow(r, 1.0) * pow(c, 1.0);
                m12 += pow(r, 1.0) * pow(c, 2.0);
            }
}

double DetectedObject::get_area() const {
    return inertia_moment(0, 0);
}

bool DetectedObject::check_for_S() const {
    if (!check_ratio_constraints(0.6, 1.5)) {
        DEBUG(std::cout << "FAIL: ratio" << std::endl;)
        return false;
    }

    auto M1 = calculate_M1();
    if (M1 < 0.26 || M1 > 0.50) {
        DEBUG(std::cout << "FAIL: M1" << std::endl;)
        return false;
    }
    auto M2 = calculate_M2();
    if (M2 < 0.01 || M2 > 0.15) {
        DEBUG(std::cout << "FAIL: M2" << std::endl;)
        return false;
    }
    auto M3 = calculate_M3();
    if (M3 < 0.005 || M3 > 0.07) {
        DEBUG(std::cout << "FAIL: M3" << std::endl;)
        return false;
    }
    auto M4 = calculate_M4();
    if (M4 < 0.0003 || M4 > 0.007) {
        DEBUG(std::cout << "FAIL: M4" << std::endl;)
        return false;
    }
    auto M5 = calculate_M5();
    auto M6 = calculate_M6();
    if (M5 * M6 < 0.0) {
        DEBUG(std::cout << "FAIL: M5 * M6" << std::endl;)
        return false;
    }
    auto M7 = calculate_M7();
    if (M7 < 0.01 || M7 > 0.038) {
        DEBUG(std::cout << "FAIL: M7" << std::endl;)
        return false;
    }

    return true;
}

bool DetectedObject::check_for_Y() const {
    if (!check_ratio_constraints(0.6, 2.3)) {
        DEBUG(std::cout << "FAIL: ratio" << std::endl;)
        return false;
    }

    auto M1 = calculate_M1();
    if (M1 < 0.29 || M1 > 0.45) {
        DEBUG(std::cout << "FAIL: M1" << std::endl;)
        return false;
    }
    auto M2 = calculate_M2();
    if (M2 < 0.01 || M2 > 0.15) {
        DEBUG(std::cout << "FAIL: M2" << std::endl;)
        return false;
    }
    auto M3 = calculate_M3();
    if (M3 < 0.01 || M3 > 0.04) {
        DEBUG(std::cout << "FAIL: M3" << std::endl;)
        return false;
    }
    auto M4 = calculate_M4();
    if (M4 > 0.005) {
        DEBUG(std::cout << "FAIL: M4" << std::endl;)
        return false;
    }
    auto M5 = calculate_M5();
    auto M6 = calculate_M6();
    if (M5 * M6 < 0.0) {
        DEBUG(std::cout << "FAIL: M5 * M6" << std::endl;)
        return false;
    }
    auto M7 = calculate_M7();
    if (M7 < 0.01 || M7 > 0.03) {
        DEBUG(std::cout << "FAIL: M7" << std::endl;)
        return false;
    }

    return true;
}

double DetectedObject::calculate_M1() const {
    auto M20 = m20 - pow(m10, 2.0) / m00;
    auto M02 = m02 - pow(m01, 2.0) / m00;

    return (M20 + M02) / pow(m00, 2.0);
}

double DetectedObject::calculate_M2() const {
    auto M20 = m20 - m10 * m10 / m00;
    auto M02 = m02 - m01 * m01 / m00;
    auto M11 = m11 - m10 * m01 / m00;

    return (pow(M20 - M02, 2.0) + 4.0 * M11 * M11) / pow(m00, 4.0);
}

double DetectedObject::calculate_M3() const {
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;
    auto y_dash = m01 / m00;
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    return (pow(M30 - 3 * M12, 2.0) + pow(3 * M21 - M03, 2.0)) / pow(m00, 5.0);
}

double DetectedObject::calculate_M4() const {
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;
    auto y_dash = m01 / m00;
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    return (pow(M30 + M12, 2.0) + pow(M21 + M03, 2.0)) / pow(m00, 5.0);
}

double DetectedObject::calculate_M5() const {
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;
    auto y_dash = m01 / m00;
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;

    return ((M30 - 3.0 * M12) * (M30 + M12) * (pow(M30 + M12, 2.0) - 3.0 * pow(M21 + M03, 2.0))
            + (3.0 * M21 - M03) * (M21 + M03) * (3 * pow(M30 + M12, 2.0) - pow(M21 + M03, 2.0))) / pow(m00, 10.0);
}

double DetectedObject::calculate_M6() const {
    auto x_dash = m10 / m00;
    auto M30 = m30 - 3 * x_dash * m20 + 2 * x_dash * x_dash * m10;
    auto y_dash = m01 / m00;
    auto M12 = m12 - 2 * y_dash * m11 - x_dash * m02 + 2 * y_dash * y_dash * m10;
    auto M21 = m21 - 2 * x_dash * m11 - y_dash * m20 + 2 * x_dash * x_dash * m01;
    auto M03 = m03 - 3 * y_dash * m02 + 2 * y_dash * y_dash * m01;
    auto M20 = m20 - x_dash * m10;
    auto M02 = m02 - y_dash * m01;
    auto M11 = m11 - x_dash * m01;

    return ((M20 - M02) * (pow(M30 + M12, 2.0) - pow(M21 + M03, 2.0))
            + 4.0 * M11 * (M30 + M12) * (M21 + M03)) / pow(m00, 7.0);
}

double DetectedObject::calculate_M7() const {
    auto M20 = m20 - pow(m10, 2.0) / m00;
    auto M02 = m02 - pow(m01, 2.0) / m00;
    auto M11 = m11 - m10 * m01 / m00;

    return (M20 * M02 - pow(M11, 2.0)) / pow(m00, 4.0);
}

std::ostream &operator<<(std::ostream &os, const DetectedObject &object) {
    auto bb = object.get_bounding_box();
    os << "image:"
       << " AR=" << std::setw(12) << static_cast<double>(object.get_area()) / (object.image.cols * object.image.rows)
       << " ratio=" << std::setw(9) << static_cast<double>(bb.height) / bb.width
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

cv::Rect DetectedObject::get_bounding_box() const {
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

cv::Point DetectedObject::get_center() const {
    auto bb = get_bounding_box();
    return {static_cast<int>(bb.x + bb.width * 0.5),
            static_cast<int>(bb.y + bb.height * 0.5)};
}

bool DetectedObject::check_ratio_constraints(double low, double high) const {
    auto bb = get_bounding_box();
    auto ratio = static_cast<double >(bb.height) / bb.width;
    return ratio > low && ratio < high;
}
