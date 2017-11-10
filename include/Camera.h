//
// Created by flamming on 2017/11/10.
//

#ifndef MUTISRCMATING_CAMERA_H
#define MUTISRCMATING_CAMERA_H

#include <opencv2/opencv.hpp>
#include <memory>

namespace qrar
{
    class Camera
    {
    public:
        Camera(double fx_=535.5, double fy_=535.5, double cx_=320, double cy_=240);
        static std::shared_ptr<Camera> getInstance();//(frome configuration or so)
        void SetParameter(double fx_, double fy_, double cx_, double cy_);

        cv::Vec2d Camera2Pixel(const cv::Vec3d& p_c);
        cv::Vec3d Pixel2Camera(const cv::Vec2d& p_p, double depth);
        const cv::Matx33d& GetK();
		double fx, fy, cx, cy;
	private:
        cv::Matx33d K;
        cv::Matx33d K_inv;
    };
    typedef std::shared_ptr<Camera> CameraPtr;
}
#endif //MUTISRCMATING_CAMERA_H
