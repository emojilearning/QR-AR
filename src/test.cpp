#include <ARDemo.h>
using namespace cv;
using namespace std;

int main()
{
	//load a example frame
	Mat qrcode = cv::imread("../QRcodeExample.png");
	Mat current_frame = cv::imread("../aframe.png");
	//vertexes of qrcode
	vector<Point2d> vertexes;
	vertexes.emplace_back(0, 0);
	vertexes.emplace_back(0, qrcode.rows);
	vertexes.emplace_back(qrcode.cols, qrcode.rows);
	vertexes.emplace_back(qrcode.cols, 0);

	//vertexes of qrcode in coming frame
	vector<Point2d> vertexes_in_frame;
	vertexes_in_frame.emplace_back(113, 223);
	vertexes_in_frame.emplace_back(256, 341);
	vertexes_in_frame.emplace_back(361, 217);
	vertexes_in_frame.emplace_back(234, 126);

	ARDemo(vertexes, vertexes_in_frame, qrcode, current_frame);

	return 0;
}
