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

void qrar::ArCube::Transform(CameraPtr observer,cv::Matx44d T)
{
	for (int i = 0; i < 8; i++)
	{
		Vec4d y = T * vertexes[i];
		scene_corners[i] = (observer->Camera2Pixel({ y(0),y(1),y(2) }));
	}
}


void qrar::ArCube::Init(CameraPtr assumpution,Vec3d normal)
{
	obj_corners[0] = { 0, 0 };
	obj_corners[1] = { 0, assumpution->cy * 2 };
	obj_corners[2] = { assumpution->cx * 2, assumpution->cy * 2 };
	obj_corners[3] = { assumpution->cx * 2, 0 };
	for (size_t i = 0; i < 4; i++)
	{
		//Mat A(3, 3, CV_64F);
		//A.at<double>(0) = normal(0);
		//A.at<double>(1) = normal(1);
		//A.at<double>(2) = normal(2);
		//A.at<double>(3) = p->fx;
		//A.at<double>(4) = 0;
		//A.at<double>(5) = p->cx - obj_corners[i].x;
		//A.at<double>(6) = 0;
		//A.at<double>(7) = p->fy;
		//A.at<double>(8) = p->cy - obj_corners[i].y;
		//Mat b(3, 1, CV_64F);
		//b.at<double>(0) = 1;
		//b.at<double>(1) = 0;
		//b.at<double>(2) = 0;
		Vec3d X;
		//cv::solve(A, b, X);

		auto ru = assumpution->cx - obj_corners[i].x;
		auto rv = assumpution->cy - obj_corners[i].y;
		auto fracbot = normal(0)*ru + normal(1)*rv + normal(2)*assumpution->fx;

		X(0) = ru / fracbot;
		X(1) = rv / fracbot;
		X(2) = assumpution->fx / fracbot;

		vertexes[2 * i] = Vec4d(X(0), X(1), X(2), 1);
		X = X - h*normal;
		vertexes[2 * i + 1] = Vec4d(X(0), X(1), X(2), 1);
	}
}

qrar::ArCube::ArCube()
{
	obj_corners.resize(4);
	scene_corners.resize(8);
	vertexes.resize(8);
	h = 0.5;
}