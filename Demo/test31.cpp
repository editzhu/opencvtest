#include <opencv2/opencv.hpp>
#include <vector>
#include<time.h>

using namespace cv;
using namespace std;

//将指定大小的图片，放大10倍

void createAlphaMat(Mat &mat){
	for (int i = 0; i < mat.rows; ++i){
		for (int j = 0; j < mat.cols; ++j){
			Vec4b&rgba = mat.at<Vec4b>(i, j);

			//rgba[0] = UCHAR_MAX;
			//rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols)*UCHAR_MAX);
			//rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.cols)*UCHAR_MAX);
			//rgba[3] = saturate_cast<uchar>(0.5*(rgba[1]+rgba[2]));
			
			rgba[0] = rand() % 255;
			rgba[1] = rand() % 255;
			rgba[2] = rand() % 255;
			rgba[3] = rand() % 255;
		}

	}
}

void add10(Mat &mat, Mat &mat10){
	for (int i = 0; i < mat.rows; ++i){
		for (int j = 0; j < mat.cols; ++j){
			int kkk = rand() % 255;
			for (int k = 0; k < 10; k++){
				for(int h = 0; h < 10; h++){
					Vec4b&rgba = mat10.at<Vec4b>(i*10+k, j*10+h);
					rgba[0] = mat.at<Vec4b>(i, j)[0];

					rgba[1] = mat.at<Vec4b>(i, j)[1];
					rgba[2] = mat.at<Vec4b>(i,j)[2];
					rgba[3] = mat.at<Vec4b>(i,j)[3];
				}
			}
			

		}
	}
}

int main31(){
	//创建带Alpha通道的Mat
	srand((int)time(0));//随机种子
	int x = 20, y = 20;
	Mat mat(x, y, CV_8UC4);
	createAlphaMat(mat);
	
	Mat mat10(x*10, y*10, CV_8UC4);

	add10(mat, mat10);
	
	vector<int>compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	try{
		//imwrite("d:\\alpha.png", mat, compression_params);
		cout << endl << mat ;
		imshow("png", mat);
		imshow("png1", mat10);
		Mat dst;
		resize(mat, dst, Size(x * 10, y * 10), 0, 0, CV_INTER_LINEAR);
		imshow("dst", dst);
		//fprintf(stdout, "can find file where D");
		waitKey(0);
	}
	catch (runtime_error& ex){
		fprintf(stderr, "error", ex.what());
		return 1;
	}
	return 0;
}