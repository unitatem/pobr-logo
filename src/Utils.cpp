//
// Created by mariusz on 24.05.18.
//

#include "Transform.h"
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

void Utils::show(const cv::Mat &image, std::string name) {
    cv::namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, image);
    cv::waitKey(-1);
}

void Utils::show_hsv(const cv::Mat &image) {
    cv::namedWindow("H", cv::WINDOW_NORMAL);
    cv::imshow("H", Transform::channels2gray(image, cv::Vec3b(1, 0, 0)));

    cv::namedWindow("S", cv::WINDOW_NORMAL);
    cv::imshow("S", Transform::channels2gray(image, cv::Vec3b(0, 1, 0)));

    cv::namedWindow("V", cv::WINDOW_NORMAL);
    cv::imshow("V", Transform::channels2gray(image, cv::Vec3b(0, 0, 1)));

    cv::waitKey(-1);
}
