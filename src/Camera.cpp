//
// Created by flamming on 2017/9/9.
//
#include "Camera.h"
using namespace cv;
namespace qrar
{
    Camera::Camera(double fx_, double fy_, double cx_, double cy_) :fx(fx_), fy(fy_), cx(cx_), cy(cy_)
    {
        K << fx, 0, cx,
                0, fy, cy,
                0, 0, 1;
        K_inv = K.inv();
    }
    std::shared_ptr<Camera> qrar::Camera::getInstance()
    {
        static std::shared_ptr<Camera> camera_ = std::make_shared<Camera>();
        return camera_;
    }

    void qrar::Camera::SetParameter(double fx_, double fy_, double cx_, double cy_)
    {
        fx = fx_;
        fy = fy_;
        cx = cx_;
        cy = cy_;
        K << fx, 0, cx,
                0, fy, cy,
                0, 0, 1;
        K_inv = K.inv();
    }

    const Matx33d & Camera::GetK()
    {
        return K;
    }
    Vec2d Camera::Camera2Pixel(const Vec3d &p_c)
    {
        Vec3d t = K*p_c;
		//LOG(INFO) << t ;
        return  Vec2d{ t[0]/t[2],t[1]/t[2] };
    }
    Vec3d Camera::Pixel2Camera(const Vec2d& p_p, double depth)
    {
        return K_inv*Vec3d(p_p[0], p_p[1], 1)*depth;
    }
}