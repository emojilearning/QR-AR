#include <opencv2/opencv.hpp>
#include <vector>
#include <HEstimator.h>
#include <ARCube.h>
#include <decompose_homography.h>

using namespace std;
using namespace qrar;
using namespace cv;

void MarkQrcode(const Mat& left, const Mat& H, Mat& out2, cv::Scalar s = cv::Scalar(0, 255, 0))
{
	std::vector<Point2d> vetexes(4);
	std::vector<Point2d> obj_corners(4);
	obj_corners[0] = Point2d(0, 0); obj_corners[1] = Point2d(left.cols, 0);
	obj_corners[2] = Point2d(left.cols, left.rows); obj_corners[3] = Point2d(0, left.rows);
	if (H.data)
	{
		perspectiveTransform(obj_corners, vetexes, H);
		line(out2, vetexes[0], vetexes[1], s, 4);
		line(out2, vetexes[1], vetexes[2], s, 4);
		line(out2, vetexes[2], vetexes[3], s, 4);
		line(out2, vetexes[3], vetexes[0], s, 4);
	}
}

void ARDemo(const vector<Point2d>& vertexes,const vector<Point2d>& vertexes_in_frame,Mat qrcode,Mat current_frame)
{
	//draw a rectangle to mark the qrcode aera
	Mat homography;
	EstimateHByDLT(vertexes, vertexes_in_frame, homography);
	Mat result1 = current_frame.clone();
	MarkQrcode(qrcode, homography, result1);
	imshow("result", result1);

	CameraPtr assume = make_shared<Camera>();
	auto f = min(qrcode.cols, qrcode.rows);
	assume->SetParameter(f, f, qrcode.cols / 2, qrcode.rows / 2);
	CameraPtr mobile_phone = make_shared<Camera>();

	vector<Mat> tContainer, nContainer, RContainer;
	qrar::DecomposeHomography(homography, assume->GetK(), mobile_phone->GetK(), RContainer, tContainer, nContainer);

	int maxIdx = -1;
	double maxd = -1;
	for (int i = 0; i < 4; ++i) {
		if ((nContainer[i].at<double>(2)) >(maxd)) {
			maxd = nContainer[i].at<double>(2);
			maxIdx = i;
		}
	}
	cv::Vec3d t = tContainer[maxIdx];
	cv::Matx33d R = RContainer[maxIdx];
	cv::Vec3d n = nContainer[maxIdx];
	cv::Matx44d T;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			T(i, j) = R(i, j);
		}
		T(i, 3) = t(i);
		T(3, i) = 0;
	}
	T(3, 3) = 1;

	ArCube cube;
	cube.Init(assume);
	cube.Transform(mobile_phone, T);
	Mat result2 = current_frame.clone();
	cube.DrawOn(result2);
	imshow("result2", result2);
	cv::waitKey(0);
}


