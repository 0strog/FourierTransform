/*
 * code from https://docs.opencv.org/master/d8/d01/tutorial_discrete_fourier_transform.html
 */

#include "fouriertransform.h"

void fourierTransform(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat padded;
    int m = cv::getOptimalDFTSize(src.rows);
    int n = cv::getOptimalDFTSize(src.cols);
    cv::copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);

    cv::dft(complexImg, complexImg);

    cv::split(complexImg, planes);
    cv::magnitude(planes[0], planes[1], planes[0]);
    dst = planes[0];

    dst += cv::Scalar::all(1);
    cv::log(dst, dst);

    dst = dst(cv::Rect(0, 0, dst.cols & -2, dst.rows & -2));

    int cx = dst.cols/2;
    int cy = dst.rows/2;

    cv::Mat q0(dst, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(dst, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(dst, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(dst, cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);


    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
}
