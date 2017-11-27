#include <opencv2/opencv.hpp>
#include <vector>
#include <ARCube.h>

using namespace std;
using namespace qrar;
using namespace cv;

//@deprecated
//use homography to marker the 2d vetexes
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

//vertexes in the order:left-top,right-top,right-bottom,left-bottom
Mat ARDemo(const Vec2d& real_size,const vector<Point2d>& vertexes_in_frame,const Mat& current_frame)
{
	int f = 535;
	int cx = 320;
	int cy = 240;
	Matx33d K;//camera_instrinct
	K << f, 0, cx,
		0, f, cy,
		0, 0, 1;

	ArCube cube(real_size[0], real_size[1]);
	cube.Init();

	cv::Vec3d t;
	Vec3d rvec;
	cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64FC1);
	solvePnP(cube.vertex3d, vertexes_in_frame, K, distCoeffs, rvec, t,false, SOLVEPNP_ITERATIVE);
	Mat r, tt;
	//solvePnP(veterxes3d, vertexes_in_frame, K, distCoeffs, r, tt, false, SOLVEPNP_UPNP);
	Matx44d T;
	cv::Matx33d R;
	cv::Rodrigues(rvec, R);
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			T(i, j) = R(i, j);
		}
		T(3, i) = t(i);
	}
	T(3, 3) = 1;
	cout << T.inv() << endl;
	cube.Transform(K, R, t);
	Mat result = current_frame.clone();
	cube.DrawOn(result);

	return result;
}


