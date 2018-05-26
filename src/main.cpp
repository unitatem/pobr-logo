#include "Detector.h"
#include "logger.h"
#include "Morphology.h"
#include "Transform.h"
#include "Utils.h"

#include "opencv2/core/core.hpp"

#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <opencv2/imgproc.hpp>

int main() {
    std::cout << "START" << std::endl;

    std::experimental::filesystem::path images_path("resources/image");
//    auto img_path = std::experimental::filesystem::directory_iterator(images_path)->path().string();
    auto img_path = "resources/image/Subway-Storefront-Insurance.jpg";
    DEBUG(std::cout << "img_path = " << img_path << std::endl;)

    auto img = Utils::load_image(img_path, cv::ImreadModes::IMREAD_COLOR);
    Utils::show(img);

    // bgr
    auto img_white = Transform::to_mono(img, cv::Vec3b(230, 230, 230), cv::Vec3b(255, 250, 250));
//    DEBUG(Utils::show(img_white);)
    auto img_white_open = Morphology::open(img_white);
    DEBUG(Utils::show(img_white_open);)

    auto img_yellow = Transform::to_mono(img, cv::Vec3b(0, 200, 220), cv::Vec3b(100, 230, 255));
//    DEBUG(Utils::show(img_yellow);)
    auto img_yellow_open = Morphology::open(img_yellow);
    DEBUG(Utils::show(img_yellow_open);)

    auto detected_white = Detector::detect(img_white_open);
    for (auto const &image : detected_white) {
        std::cout << image;
//        Utils::show(image.get_image());
    }

    auto detected_yellow = Detector::detect(img_yellow_open);
    for (auto const &image : detected_yellow) {
        std::cout << image;
//        Utils::show(image.get_image());
    }

    DEBUG(std::cout << std::endl;)
    detected_white = Detector::filter(detected_white);
    for (auto const &image : detected_white) {
        std::cout << image;
//        Utils::show(image.get_image());
    }
    detected_yellow = Detector::filter(detected_yellow);
    for (auto const &image : detected_yellow) {
        std::cout << image;
//        Utils::show(image.get_image());
    }

    detected_white.insert(detected_white.end(), detected_yellow.begin(), detected_yellow.end());

    cv::Rect bounding_box(detected_white[0].get_image().cols, detected_white[0].get_image().rows, -detected_white[0].get_image().cols, -detected_white[0].get_image().rows);
    for (auto &image : detected_white) {
        auto bb = image.find_bounding_box();
        bounding_box |= bb;
    }

    std::cout << bounding_box;

    cv::rectangle(img, bounding_box, cv::Scalar(0, 0, 255));
    Utils::show(img);

    return 0;
}