#include "logger.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <experimental/filesystem>
#include <iostream>
#include <iterator>

cv::Mat load_image(const std::string &path, int mode) {
    auto image = cv::imread(path, mode);
    if (!image.data) {
        std::cout << "Could not open or find the image" << std::endl;
        exit(-1);
    }
    return image;
}

void show_image(const cv::Mat &image) {
    cv::imshow("tmp", image);
    cv::waitKey(-1);
}

int main() {
    std::cout << "START" << std::endl;

    std::experimental::filesystem::path images_path("resources/image");
    auto img_path = std::experimental::filesystem::directory_iterator(images_path)->path().string();
    DEBUG(std::cout << "img_path = " << img_path << std::endl;)

    auto image = load_image(img_path, cv::ImreadModes::IMREAD_COLOR);
    show_image(image);

    return 0;
}