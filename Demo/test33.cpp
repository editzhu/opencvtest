#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//mat �� mat.data ������

int main33(){
	Mat M(2, 2, CV_8UC3, Scalar(0, 0, 255));
	cout << "M = " << endl << " " << M << endl << endl;

	for (int i = 0; i < M.rows * M.cols; i++)
	{
		float* data = (float*)M.data;
		cout << data[i] << " ";
	}
	getchar();

	return 0;
}