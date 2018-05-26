//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_UTILS_H
#define POBR_LOGO_UTILS_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class Utils {
public:
    static cv::Mat load_image(const std::string &path, int mode);

    static void show(const cv::Mat &image, std::string name = "tmp");
    static void show_hsv(const cv::Mat &image);
};


#endif //POBR_LOGO_UTILS_H
