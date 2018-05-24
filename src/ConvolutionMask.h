//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_CONVOLUTIONMASK_H
#define POBR_LOGO_CONVOLUTIONMASK_H

#include "opencv2/core/core.hpp"

class ConvolutionMask {
public:
    static ConvolutionMask ones;

    ConvolutionMask(const int *mask, int rows, int cols);
    int getBorderRows() const;
    int getBorderCols() const;

    int convolve(cv::Mat image, int row, int col) const;

private:
    const int *mask;
    const int rows;
    const int cols;
};


#endif //POBR_LOGO_CONVOLUTIONMASK_H
