#include <opencv2/opencv.hpp>
#include <vector>
#include<time.h>

using namespace cv;
using namespace std;

//��ָ��ͼƬ���ŵ�ָ����С��������
#define fffsize 100
//��ָ�������С�Ķ�ֵͼ���Ŵ�10��
#define bei 3

void add3210(Mat &mat, Mat &mat10){
	for (int i = 0; i < mat.rows; ++i){
		for (int j = 0; j < mat.cols; ++j){
			int kkk = rand() % 255;
			for (int k = 0; k < bei; k++){
				for (int h = 0; h < bei; h++){
					uchar&rgba = mat10.at<uchar>(i * bei + k, j * bei + h);
					rgba= mat.at<uchar>(i, j);

	
				}
			}


		}
	}
}

int main32(){
	Mat mat = imread("d:\\daolu1.jpg", 0);//�ڰ�
	Mat dst;
	//��dst��С��16*16
	resize(mat, dst, Size(fffsize, fffsize), 0, 0, CV_INTER_LINEAR);
	//��m2ת��Ϊ��ֵͼ,typeѡTHRESH_BINARY��������ֵ������Ϊmaxval(255),������0  
	//threshold(dst, dst, 96, 255, THRESH_BINARY);

	Mat mat10(dst.rows * 10, dst.cols * 10, CV_8UC1);

	add3210(dst, mat10);


	try{
		//imwrite("d:\\alpha.png", mat, compression_params);
		//cout << endl << dst << endl;
		imshow("mat", mat);
		imshow("dst", dst);
		imshow("mat10", mat10);
		//fprintf(stdout, "can find file where D");
		waitKey(0);
	}
	catch (runtime_error& ex){
		fprintf(stderr, "error", ex.what());
		return 1;
	}
	return 0;
}