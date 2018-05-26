//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_DETECTEDOBJECT_H
#define POBR_LOGO_DETECTEDOBJECT_H

#include <ostream>
#include "opencv2/core/core.hpp"

class DetectedObject {
public:
    DetectedObject(const cv::Mat &image);

    cv::Mat get_image() const;
    unsigned long long int get_area() const;

    bool check_for_S();
    bool check_for_Y();

    double calculate_M1() const;
    double calculate_M7() const;
    double calculate_M2() const;
    double calculate_M3() const;
    double calculate_M4() const;
    double calculate_M5() const;
    double calculate_M6() const;

    cv::Rect find_bounding_box();

    friend std::ostream &operator<<(std::ostream &os, const DetectedObject &object);

private:
    cv::Mat image;

    unsigned long long int inertia_moment(int vertical_order, int horizontal_order) const;
};


#endif //POBR_LOGO_DETECTEDOBJECT_H
