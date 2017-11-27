#include <ARDemo.h>
using namespace cv;
using namespace std;

int main()
{
	const double width = 1;
	const double height = 1;
	//load a example frame
	Mat current_frame = cv::imread("../aframe.png");
	//Mat current_frame = cv::imread("../test.png");
	//vertex3d of qrcode in coming frame
	vector<Point2d> vertexes_in_frame;
	//vertexes_in_frame.emplace_back(232, 81);
	//vertexes_in_frame.emplace_back(437, 110);
	//vertexes_in_frame.emplace_back(438, 388);
	//vertexes_in_frame.emplace_back(233, 413);
	vertexes_in_frame.emplace_back(113, 223);
	vertexes_in_frame.emplace_back(256, 341);
	vertexes_in_frame.emplace_back(361, 217);
	vertexes_in_frame.emplace_back(234, 126);
	//vertexes_in_frame.emplace_back(200, 120);
	//vertexes_in_frame.emplace_back(520, 120);
	//vertexes_in_frame.emplace_back(520, 360);
	//vertexes_in_frame.emplace_back(200, 360);
	auto result = ARDemo({ width,height }, vertexes_in_frame, current_frame);
	imshow("result", result);
	cv::waitKey(0);
	return 0;
}
