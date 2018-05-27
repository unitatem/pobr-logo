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
    double get_area() const;

    bool check_for_S();
    bool check_for_Y();

    double calculate_M1() const;
    double calculate_M7() const;
    double calculate_M2() const;
    double calculate_M3() const;
    double calculate_M4() const;
    double calculate_M5() const;
    double calculate_M6() const;

    cv::Rect find_bounding_box() const;

    friend std::ostream &operator<<(std::ostream &os, const DetectedObject &object);

private:
    cv::Mat image;

    double m00;
    double m10, m20, m30;
    double m01, m02, m03;
    double m21, m11, m12;

    double inertia_moment(int vertical_order, int horizontal_order) const;
    void calculate_all_inertia_moment();

    bool check_ratio_contraints(double low, double high) const;
};


#endif //POBR_LOGO_DETECTEDOBJECT_H
