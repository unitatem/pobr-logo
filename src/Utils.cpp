//
// Created by mariusz on 24.05.18.
//

#include "Utils.h"

#include <iostream>

cv::Mat Utils::load_image(const std::string &path, int mode) {
    auto image = cv::imread(path, mode);
    if (!image.data) {
        std::cout << "Could not open or find the image" << std::endl;
        exit(-1);
    }
    return image;
}

void Utils::show(const cv::Mat &image) {
    cv::imshow("tmp", image);
    cv::waitKey(-1);
}
