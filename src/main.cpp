#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

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

    const std::string  lena_path = "resources/Lena.png";
    auto image = load_image(lena_path, cv::ImreadModes::IMREAD_COLOR);
    show_image(image);

    return 0;
}