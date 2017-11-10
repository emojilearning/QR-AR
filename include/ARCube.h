//
// Created by flamming on 2017/11/10.
//

#ifndef MUTISRCMATING_ARBOX_H
#define MUTISRCMATING_ARBOX_H
#include <Camera.h>
#include <opencv2/opencv.hpp>
namespace qrar
{
	class ArCube
	{
	public:
		ArCube();
		void Init(CameraPtr assumpution, cv::Vec3d normal = { 0,0,1 });
		void DrawOn(cv::Mat);
		void Transform(CameraPtr observer, cv::Matx44d T);
		std::vector<cv::Point2d> obj_corners;
		std::vector<cv::Vec4d> vertexes;
		std::vector<cv::Point2d> scene_corners;
	private:
		double h;
	};
}

#endif //MUTISRCMATING_ARBOX_H
