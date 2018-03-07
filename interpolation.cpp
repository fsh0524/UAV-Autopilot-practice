#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

using namespace std;
using namespace cv;

void 	bilinear_interpolation(Mat& input, Mat& output, float scalingFactor);

bool equal(const Mat & a, const Mat & b)
{
    if ( (a.rows != b.rows) || (a.cols != b.cols) )
        return false;
    Scalar s = sum( a - b );
    // for (int i = 0 ; i < a.rows ; ++i) {
    // 	for (int j = 0 ; j < a.cols ; ++j) {
    // 		if (a.at<Vec3b>(i, j) != b.at<Vec3b>(i, j)) {
    // 			cerr << "Fuck: " << i << ", " << j << endl;
    // 		}
    // 	}
    // }
    return (s[0]==0) && (s[1]==0) && (s[2]==0);
}

int 	main(int argc, char** argv) {
	
	Mat inputImg = imread(argv[1]);
	float scalingFactor = atof(argv[2]);
	
	int scaledWidth = round(1. * inputImg.cols * scalingFactor);
	int scaledHeight = round(1. * inputImg.rows * scalingFactor);
	
	Mat outputImg1 = Mat(scaledHeight, scaledWidth, inputImg.type());
	Mat outputImg2; // for opencv build-in function
	
	// resize the input image by your bilinear_interpolation funcion
	bilinear_interpolation(inputImg, outputImg1, scalingFactor);
	// resize the input image by opencv
	// outputImg2 = ....
	resize(inputImg, outputImg2, Size(scaledWidth, scaledHeight), 0, 0, INTER_LINEAR);

	imshow("My Interpolation", outputImg1);
	imshow("Opencv build-in function", outputImg2);
	waitKey(0);

	// imwrite("output.jpg", outputImg1);

	cerr << (equal(outputImg1, outputImg2) ? "Yes" : "No") << endl;
	return 0;
}


void 	bilinear_interpolation(Mat& input, Mat& output, float scalingFactor) {
	
	// write down your code here
	for (int i = 0 ; i < output.rows ; ++i) {
		for (int j = 0 ; j < output.cols ; ++j) {
			for (int k = 0 ; k < 3 ; ++k) {
				int x1 = i / scalingFactor;
				int x2 = min(x1 + 1, input.rows-1);
				int y1 = j / scalingFactor;
				int y2 = min(y1 + 1, input.cols-1);
				auto a = input.at<Vec3b>(x1, y1)[k];
				auto b = input.at<Vec3b>(x2, y1)[k];
				auto c = input.at<Vec3b>(x1, y2)[k];
				auto d = input.at<Vec3b>(x2, y2)[k];
				float dx = i - int(i / scalingFactor) * scalingFactor;
				float dy = j - int(j / scalingFactor) * scalingFactor;
				output.at<Vec3b>(i, j)[k]
					= ((float)a * (scalingFactor-dx) * (scalingFactor-dy)
					+ (float)b * (dx) * (scalingFactor-dy)
					+ (float)c * (scalingFactor-dx) * (dy)
					+ (float)d * (dx) * (dy)) / (scalingFactor * scalingFactor);
			}
		}
	}

}

