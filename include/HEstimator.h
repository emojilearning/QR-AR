//
// Created by flamming on 01/07/2017.
//
#ifndef MARKERAR_HESTIMATOR_H
#define MARKERAR_HESTIMATOR_H
#include <opencv2/opencv.hpp>
bool EstimateHByDLT(const std::vector<cv::Point2d>& lpts, const std::vector<cv::Point2d>& rpts, cv::Mat& model);
bool EstimateHByDLT(const cv::Mat& lpts, const cv::Mat& rpts, cv::Mat& H);
bool IsFineH(cv::Mat H);
bool HaveCollinearPoints(const cv::Mat& m, int count);
bool CheckSubset(const cv::Mat& ms1, const cv::Mat& ms2);

#endif // !MARKERAR_HESTIMATOR_H


