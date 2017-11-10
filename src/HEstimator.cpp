#include <HEstimator.h>

using std::vector;
using cv::Point2d;
using cv::Mat;
using cv::determinant;
using cv::Matx33d;

bool IsFineH(Mat H)
{
	//no mirror. det = lambda1*lambda2,this could be used to verify ...
	double det = H.at<double>(0, 0) * H.at<double>(1, 1) - H.at<double>(1, 0) * H.at<double>(0, 1);
	//Mat A = H(cv::Rect(0, 0, 2, 2));
	//Mat matW(2, 1, CV_64F);
	//Mat matU(2, 2, CV_64F);
	//Mat matV(2, 2, CV_64F);
	//cv::SVDecomp(A, matW, matU, matV);
	//std::cout << matW<<matU<<matV << std::endl;
	if (det < 0)
		return false;
	//verify the scale ratio of different directions,this could be used to verify...
	double N1 = sqrt(H.at<double>(0, 0) * H.at<double>(0, 0) + H.at<double>(1, 0) * H.at<double>(1, 0));
	if (N1 > 4)
		return false;
	if (N1 < 0.1)
		return false;
	double N2 = sqrt(H.at<double>(0, 1) * H.at<double>(0, 1) + H.at<double>(1, 1) * H.at<double>(1, 1));
	if (N2 > 4)
		return false;
	if (N2 < 0.1)
		return false;
	//verify the projective factor
	double N3 = sqrt(H.at<double>(2, 0) * H.at<double>(2, 0) + H.at<double>(2, 1) * H.at<double>(2, 1));
	if (N3 > 0.002)
		return false;
	return true;
}

bool HaveCollinearPoints(const Mat& m, int count)
{
	int j, k, i = count - 1;
	const Point2d* ptr = m.ptr<Point2d>();

	// check that the i-th selected point does not belong
	// to a line connecting some previously selected points
	// also checks that points are not too close to each other
	for (j = 0; j < i; j++)
	{
		double dx1 = ptr[j].x - ptr[i].x;
		double dy1 = ptr[j].y - ptr[i].y;
		for (k = 0; k < j; k++)
		{
			double dx2 = ptr[k].x - ptr[i].x;
			double dy2 = ptr[k].y - ptr[i].y;
			if (fabs(dx2*dy1 - dy2*dx1) <= FLT_EPSILON*(fabs(dx1) + fabs(dy1) + fabs(dx2) + fabs(dy2)))
				return true;
		}
	}
	return false;
}
bool CheckSubset(const Mat& ms1, const Mat& ms2)
{
	if (HaveCollinearPoints(ms1, 4) || HaveCollinearPoints(ms2, 4))
		return false;

	//Check whether the any 3 points reserve their relative position
	static const int tt[][3] = { { 0, 1, 2 },{ 1, 2, 3 },{ 0, 2, 3 },{ 0, 1, 3 } };
	const Point2d* src = ms1.ptr<Point2d>();
	const Point2d* dst = ms2.ptr<Point2d>();
	int negative = 0;

	for (int i = 0; i < 4; i++)
	{
		const int* t = tt[i];
		Matx33d A(src[t[0]].x, src[t[0]].y, 1., src[t[1]].x, src[t[1]].y, 1., src[t[2]].x, src[t[2]].y, 1.);
		Matx33d B(dst[t[0]].x, dst[t[0]].y, 1., dst[t[1]].x, dst[t[1]].y, 1., dst[t[2]].x, dst[t[2]].y, 1.);
		negative += determinant(A)*determinant(B) < 0;
	}
	if (negative != 0 && negative != 4)
		return false;

	return true;
}
bool EstimateHByDLT(const std::vector<cv::Point2d>& lpts, const std::vector<cv::Point2d>& rpts, cv::Mat& model)
{
	return EstimateHByDLT(Mat(lpts), Mat(rpts), model);
}

bool EstimateHByDLT(const cv::Mat& lpts, const cv::Mat& rpts, cv::Mat& model)
{
	if (!CheckSubset(lpts, rpts))
		return false;

	if (lpts.rows != rpts.rows)
		return false;
	int n = lpts.rows;
	Mat leftpts = lpts.clone();
	Mat rightpts = rpts.clone();
	Point2d* lp = leftpts.ptr<Point2d>();
	Point2d* rp = rightpts.ptr<Point2d>();
	Point2d meanl(0, 0), meanr(0, 0), sl(0, 0), sr(0, 0);
	for (size_t i = 0; i < n; i++)
	{
		meanl += lp[i];
		meanr += rp[i];
	}
	meanl /= n;
	meanr /= n;
	Point2d t;
	for (size_t i = 0; i < n; i++)
	{
		t = lp[i] - meanl;
		sl.x += fabs(t.x);
		sl.y += fabs(t.y);
		t = rp[i] - meanr;
		sr.x += fabs(t.x);
		sr.y += fabs(t.y);
	}
	sl = sl / n;
	sr = sr / n;
	if (fabs(sl.x) < DBL_EPSILON || fabs(sl.y) < DBL_EPSILON ||
		fabs(sr.x) < DBL_EPSILON || fabs(sr.y) < DBL_EPSILON)
		return false;

	double _invHnormr[9] = { sr.x, 0, meanr.x, 0, sr.y, meanr.y, 0, 0, 1 };
	double _Hnorml[9] = { 1. / sl.x, 0, -meanl.x*1. / sl.x, 0, 1. / sl.y, -meanl.y*1. / sl.y, 0, 0, 1 };
	Mat invHnormr(3, 3, CV_64FC1, _invHnormr);
	Mat Hnorml(3, 3, CV_64FC1, _Hnorml);

	for (size_t i = 0; i < n; i++)
	{
		lp[i] -= meanl;
		lp[i].x /= sl.x;
		lp[i].y /= sl.y;
		rp[i] -= meanr;
		rp[i].x /= sr.x;
		rp[i].y /= sr.y;
	}
	double ATA[9][9];
	Mat ATAM(9, 9, CV_64F, &ATA[0][0]);
	ATAM.setTo(cv::Scalar::all(0));
	for (size_t i = 0; i < n; i++)
	{
		double X1 = lp[i].x, Y1 = lp[i].y;
		double X2 = rp[i].x, Y2 = rp[i].y;
		double A1[] = { X1,Y1,1,/*1~3*/0,0,0,/*4~6*/-X2*X1 ,-X2*Y1,-X2 };
		double A2[] = { 0,0,0,/*1~3*/X1,Y1,1,/*4~6*/-Y2*X1 ,-Y2*Y1,-Y2 };
		for (int j = 0; j < 9; j++)
			for (int k = j; k < 9; k++)
				ATA[j][k] += A1[j] * A1[k] + A2[j] * A2[k];
	}
	cv::completeSymm(ATAM);

	double  V[9][9];
	Mat matW(9, 1, CV_64F);
	Mat matV(9, 9, CV_64F, V);
	Mat _H0(3, 3, CV_64F, V[8]);

	cv::eigen(ATAM, matW, matV);

	Mat tempH(3, 3, CV_64F);

	tempH = invHnormr*_H0*Hnorml;
	tempH.convertTo(model, tempH.type(), 1. / tempH.at<double>(2, 2));
	if (IsFineH(tempH))
		return true;
	return false;
}


