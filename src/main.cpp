#include "Detector.h"
#include "logger.h"
#include "Morphology.h"
#include "Segment.h"
#include "Utils.h"

#include "opencv2/core/core.hpp"

#include <experimental/filesystem>
#include <iostream>
#include <iterator>

int main() {
    std::cout << "START" << std::endl;

    std::experimental::filesystem::path images_path("resources/image");
    auto img_path = std::experimental::filesystem::directory_iterator(images_path)->path().string();
    DEBUG(std::cout << "img_path = " << img_path << std::endl;)

    auto img = Utils::load_image(img_path, cv::ImreadModes::IMREAD_COLOR);
    Utils::show(img);

    // bgr
    auto img_white = Segment::to_mono(img, cv::Vec3b(230, 230, 230), cv::Vec3b(255, 250, 250));
    DEBUG(Utils::show(img_white);)
    auto img_white_open = Morphology::open(img_white);
    DEBUG(Utils::show(img_white_open);)

    auto img_yellow = Segment::to_mono(img, cv::Vec3b(0, 200, 220), cv::Vec3b(100, 230, 255));
    DEBUG(Utils::show(img_yellow);)
    auto img_yellow_open = Morphology::open(img_yellow);
    DEBUG(Utils::show(img_yellow_open);)

    auto detected_white = Detector::detect(img_white_open);
    for (auto const &image : detected_white)
        Utils::show(image.get_image());

    auto detected_yellow = Detector::detect(img_yellow_open);
    for (auto const &image : detected_yellow)
        Utils::show(image.get_image());

    return 0;
}