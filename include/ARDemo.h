//
// Created by flamming on 2017/11/10.
//
#ifndef QRAR_ARDEMO_H
#define QRAR_ARDEMO_H
#include <opencv2/opencv.hpp>

cv::Mat ARDemo(const cv::Vec2d& real_size, const std::vector<cv::Point2d>& vertexes_in_frame, const cv::Mat& current_frame);
#endif