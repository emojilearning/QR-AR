//
// Created by flamming on 2017/11/10.
//

#ifndef QRAR_ARCUBE_H
#define QRAR_ARCUBE_H
#include <opencv2/opencv.hpp>
namespace qrar
{
	class ArCube
	{
	public:
		ArCube(double length = 1, double width = 1);
		void Init();
		void DrawOn(cv::Mat);
		void Transform(const cv::Matx33d& K,const cv::Matx33d& R,const cv::Vec3d& t);
		std::vector<cv::Point2d> obj_corners;
		std::vector<cv::Vec3d> vertex3d;
		std::vector<cv::Point2d> scene_corners;
	private:
		double depth;//height of the cube
		double width_, height_;
	};
}

#endif //MUTISRCMATING_ARBOX_H
