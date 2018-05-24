#include "logger.h"
#include "Morphology.h"

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

bool op_ge(const cv::Vec3b &value, const cv::Vec3b &ref) {
    return value[0] >= ref[0] && value[1] >= ref[1] && value[2] >= ref[2];
}

bool op_le(const cv::Vec3b &value, const cv::Vec3b &ref) {
    return value[0] <= ref[0] && value[1] <= ref[1] && value[2] <= ref[2];
}

cv::Mat segment_threshold_to_rgb(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC3, cv::Vec3b(0, 0, 0));

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            const auto &pixel = image.at<cv::Vec3b>(r, c);
            if (op_ge(pixel, low) && op_le(pixel, high))
                result.at<cv::Vec3b>(r, c) = pixel;
        }

    return result;
}

cv::Mat segment_threshold_to_mono(const cv::Mat &image, const cv::Vec3b &low, const cv::Vec3b &high) {
    assert(image.channels() == 3);
    cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    for (auto r = 0; r < image.rows; ++r)
        for (auto c = 0; c < image.cols; ++c) {
            const auto &pixel = image.at<cv::Vec3b>(r, c);
            if (op_ge(pixel, low) && op_le(pixel, high))
                result.at<uchar >(r, c) = 255;
        }

    return result;
}

int main() {
    std::cout << "START" << std::endl;

    std::experimental::filesystem::path images_path("resources/image");
    auto img_path = std::experimental::filesystem::directory_iterator(images_path)->path().string();
    DEBUG(std::cout << "img_path = " << img_path << std::endl;)

    auto img = load_image(img_path, cv::ImreadModes::IMREAD_COLOR);
    show_image(img);

    // bgr
    auto img_white = segment_threshold_to_mono(img, cv::Vec3b(230, 230, 230), cv::Vec3b(250, 250, 240));
    DEBUG(show_image(img_white);)
    auto img_white_open = Morphology::open(img_white);
    DEBUG(show_image(img_white_open);)

    auto img_yellow = segment_threshold_to_mono(img, cv::Vec3b(0, 200, 240), cv::Vec3b(100, 230, 255));
    DEBUG(show_image(img_yellow);)
    auto img_yellow_open = Morphology::open(img_yellow);
    DEBUG(show_image(img_yellow_open);)

    return 0;
}