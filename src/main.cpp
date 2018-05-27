#include "Segment.h"
#include "logger.h"
#include "Morphology.h"
#include "Transform.h"
#include "Utils.h"

#include <experimental/filesystem>
#include <iterator>
#include <opencv2/imgproc.hpp>

int main() {
    std::cout << "START" << std::endl;

    std::experimental::filesystem::path images_path("resources/image");
//    auto img_path = "resources/image/photo_name";
    for (const auto &path : std::experimental::filesystem::directory_iterator(images_path)) {
        auto img_path = path.path().string();
        INFO_ENABLE(std::cout << "img_path = " << img_path << std::endl;)

        auto img = Utils::load_image(img_path, cv::ImreadModes::IMREAD_COLOR);
        INFO_ENABLE(Utils::show(img, "original");)

        cv::Mat img_hsv = Transform::rgb2hsv(img);
        DEBUG_ENABLE(Utils::show_hsv(img_hsv);)

        auto img_S = Transform::channels2black(img_hsv, cv::Vec3b(180, 0, 170), cv::Vec3b(70, 70, 255));
        DEBUG_ENABLE(Utils::show(img_S);)
        auto img_S_open = Morphology::open(img_S);
        DEBUG_ENABLE(Utils::show(img_S_open, "S_open");)

        auto img_Y = Transform::channels2black(img_hsv, cv::Vec3b(120, 70, 150), cv::Vec3b(150, 255, 255));
        DEBUG_ENABLE(Utils::show(img_Y);)
        auto img_Y_open = Morphology::open(img_Y);
        DEBUG_ENABLE(Utils::show(img_Y_open, "Y_open");)

        auto segments_S = Segment::segment(img_S_open);
//        for (auto const &image : segments_S) {
//            std::cout << image;
//            Utils::show(image.get_image());
//        }

        auto segments_Y = Segment::segment(img_Y_open);
//        for (auto const &image : segments_Y) {
//            std::cout << image;
//            Utils::show(image.get_image());
//        }

        segments_S = Segment::filter_for_S(segments_S);
        DEBUG_ENABLE(
                static auto idx = 0;
                std::cout << "S" << std::endl;
                for (auto const &detected : segments_S) {
                    std::cout << idx++ << detected;
                    detected.check_for_S();
                    Utils::show(detected.get_image());
                })
        segments_Y = Segment::filter_for_Y(segments_Y);
        DEBUG_ENABLE(
                std::cout << "Y" << std::endl;
                for (auto const &detected : segments_Y) {
                    std::cout << idx++ << detected;
                    detected.check_for_Y();
                    Utils::show(detected.get_image());
                })

        auto best_matching_pair = Segment::find_best_pair(segments_S, segments_Y);

        cv::Rect bounding_box;
        for (auto &image : best_matching_pair) {
            auto bb = image.get_bounding_box();
            bounding_box |= bb;
        }
        INFO_ENABLE(std::cout << bounding_box << "\n" << std::endl;)

        cv::rectangle(img, bounding_box, cv::Scalar(0, 0, 255), 3);
        INFO_ENABLE(Utils::show(img, "final");)
    }

    return 0;
}