//
// Created by flamming on 2017/9/9.
//
#include <ARCube.h>
using namespace cv;

void qrar::ArCube::DrawOn(cv::Mat img)
{
	for (int i = 0; i < 4; i++)
	{
		line(img, scene_corners[2 * i], scene_corners[2 * i + 1], Scalar(255, 0, 0), 4);
		line(img, scene_corners[2 * i], scene_corners[(2 * i + 2) % 8], Scalar(255, 0, 0), 4);
		line(img, scene_corners[2 * i + 1], scene_corners[(2 * i + 3) % 8], Scalar(255, 0, 0), 4);
	}
}

void qrar::ArCube::Transform(const cv::Matx33d& K,const cv::Matx33d& R, const cv::Vec3d& t)
{
	for (int i = 0; i < 8; i++)
	{
		Vec3d y = R * vertex3d[i] + t;
		Vec3d t = K*y;
		scene_corners[i] = { t[0] / t[2],t[1] / t[2] };
	}
}

void qrar::ArCube::Init() 
{
	vertex3d[0] = { -width_ / 2.0, height_ / 2, 1 };
	vertex3d[1] = { -width_ / 2.0, height_ / 2, 1 - depth };
	vertex3d[2] = { width_ / 2.0, height_ / 2, 1 };
	vertex3d[3] = { width_ / 2.0, height_ / 2, 1 - depth };
	vertex3d[4] = { width_ / 2.0, -height_ / 2, 1 };
	vertex3d[5] = { width_ / 2.0, -height_ / 2, 1 - depth };
	vertex3d[6] = { -width_ / 2.0, -height_ / 2, 1 };
	vertex3d[7] = { -width_ / 2.0, -height_ / 2, 1 - depth };
}


qrar::ArCube::ArCube(double width, double height) :width_(width), height_(height)
{
	obj_corners.resize(4);
	scene_corners.resize(8);
	vertex3d.resize(8);
	depth = 0.5;
}