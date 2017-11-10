#ifndef QRAR_DECOMPOSE_HOMOGRAPHY_H
#define QRAR_DECOMPOSE_HOMOGRAPHY_H
#include "opencv2/core/utility.hpp"
namespace qrar
{
	using namespace cv;
	int DecomposeHomography(InputArray _H,
		InputArray _K1,
		InputArray _K2,
		OutputArrayOfArrays _rotations,
		OutputArrayOfArrays _translations,
		OutputArrayOfArrays _normals);
}
#endif // !QRAR_DECOMPOSE_HOMOGRAPHY_H
