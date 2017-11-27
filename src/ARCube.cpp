//
// Created by flamming on 2017/9/9.
//
#include <ARCube.h>
using namespace cv;
using namespace std;
void qrar::ArCube::DrawOn(cv::Mat img)
{
	for (int i = 0; i < 4; i++)
	{
		line(img, scene_corners[2 * i], scene_corners[2 * i + 1], Scalar(0, 255, 255), 2);
		line(img, scene_corners[2 * i], scene_corners[(2 * i + 2) % 8], Scalar(0, 255, 255), 2);
		line(img, scene_corners[2 * i + 1], scene_corners[(2 * i + 3) % 8], Scalar(0, 255, 255), 2);
	}
}

void qrar::ArCube::Transform(const cv::Matx33d& K,const cv::Matx33d& R, const cv::Vec3d& t)
{
	for (int i = 0; i < 4; i++)
	{
		Vec3d y = R * vertex3d[i] + t;
		Vec3d xt = K*y;
		scene_corners[2 * i] = { xt[0] / xt[2],xt[1] / xt[2] };
		if((R*Vec3d{ 0,0,-1 })[2]<0)
			y = y + R*Vec3d{0,0,-1}*0.5;
		else
			y = y - R*Vec3d{ 0,0,-1 }*0.5;
		xt = K*y;
		scene_corners[2 * i +1] = { xt[0] / xt[2],xt[1] / xt[2] };
	}
}

void qrar::ArCube::Init() 
{
	//vertex3d[0] = { -width_ / 2.0, height_ / 2, 1 };
	//vertex3d[1] = { width_ / 2.0, height_ / 2, 1 };
	//vertex3d[2] = { width_ / 2.0, -height_ / 2, 1 };
	//vertex3d[3] = { -width_ / 2.0, -height_ / 2, 1 };

	vertex3d[3] = { -width_ / 2.0, height_ / 2, 1 };
	vertex3d[2] = { width_ / 2.0, height_ / 2, 1 };
	vertex3d[1] = { width_ / 2.0, -height_ / 2, 1 };
	vertex3d[0] = { -width_ / 2.0, -height_ / 2, 1 };
}


qrar::ArCube::ArCube(double width, double height) :width_(width), height_(height)
{
	obj_corners.resize(4);
	scene_corners.resize(8);
	vertex3d.resize(4);
	depth = 0.5;
}