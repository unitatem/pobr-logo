//
// Created by mariusz on 24.05.18.
//

#include "ConvolutionMask.h"

namespace mask {
    const int ones[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
}

ConvolutionMask ConvolutionMask::ones(&mask::ones[0], 3, 3);

ConvolutionMask::ConvolutionMask(const int *mask, int rows, int cols)
    : mask(mask), rows(rows), cols(cols)
{}

int ConvolutionMask::convolve(cv::Mat image, int row, int col) const {
    row -= static_cast<int>(rows * 0.5);
    col -= static_cast<int>(cols * 0.5);
    auto value = 0u;

    auto mask_idx = 0;
    for (auto r = 0; r < rows; ++r)
        for (auto c = 0; c < cols; ++c)
            value += image.at<uchar>(row + r, col + c) * mask[mask_idx++];

    return value;
}

int ConvolutionMask::getBorderRows() const {
    return 0.5 * rows;
}

int ConvolutionMask::getBorderCols() const {
    return 0.5 * cols;
}


