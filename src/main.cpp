#include "Segment.h"
#include "logger.h"
#include "Morphology.h"
#include "Transform.h"
#include "Utils.h"

#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <opencv2/imgproc.hpp>

int main() {
    std::cout << "START" << std::endl;

    std::experimental::filesystem::path images_path("resources/image");
//    auto img_path = "resources/image/Subway_2.jpg";
    for (const auto &img_path : std::experimental::filesystem::directory_iterator(images_path)) {
        DEBUG(std::cout << "img_path = " << img_path << std::endl;)

        auto img = Utils::load_image(img_path.path().string(), cv::ImreadModes::IMREAD_COLOR);
        Utils::show(img, "original");

        cv::Mat img_hsv = Transform::rgb2hsv(img);
        Utils::show_hsv(img_hsv);

        auto img_white = Transform::channels2black(img_hsv, cv::Vec3b(120, 0, 170), cv::Vec3b(200, 40, 255));
        DEBUG(Utils::show(img_white);)
        auto img_white_open = Morphology::open(img_white);
        DEBUG(Utils::show(img_white_open, "white");)

        auto img_yellow = Transform::channels2black(img_hsv, cv::Vec3b(32, 150, 150), cv::Vec3b(44, 255, 255));
        DEBUG(Utils::show(img_yellow);)
        auto img_yellow_open = Morphology::open(img_yellow);
        DEBUG(Utils::show(img_yellow_open, "yellow");)

        auto detected_white = Segment::segment(img_white_open);
//        for (auto const &image : detected_white) {
//            std::cout << image;
//            Utils::show(image.get_image());
//        }

        auto detected_yellow = Segment::segment(img_yellow_open);
//        for (auto const &image : detected_yellow) {
//            std::cout << image;
//            Utils::show(image.get_image());
//        }

        static auto idx = 0;
        DEBUG(std::cout << "S" << std::endl;)
        detected_white = Segment::filter_for_S(detected_white);
        for (auto const &detected : detected_white) {
            std::cout << idx++ << detected;
            Utils::show(detected.get_image());
        }
        DEBUG(std::cout << "Y" << std::endl;)
        detected_yellow = Segment::filter_for_Y(detected_yellow);
        for (auto const &detected : detected_yellow) {
            std::cout << idx++ << detected;
            Utils::show(detected.get_image());
        }

        detected_white.insert(detected_white.end(), detected_yellow.begin(), detected_yellow.end());

        cv::Rect bounding_box;
        for (auto &image : detected_white) {
            auto bb = image.find_bounding_box();
            bounding_box |= bb;
        }

        std::cout << bounding_box << std::endl;
        cv::rectangle(img, bounding_box, cv::Scalar(0, 0, 255));
        Utils::show(img, "final");
    }

    return 0;
}