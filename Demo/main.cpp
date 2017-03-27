#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>  

using namespace cv;
using namespace std;

const char *dTitle = "二值图";
const char *sTitle = "原图";
const char *barName = "阀值";

Mat ss;
Mat dd;
void on_trackbar(int pos)
{
	//将图片二值化
	threshold(ss, dd, pos, 255, THRESH_BINARY);
	imshow(dTitle, dd);
}

void main00(void){

	//	IMTEAD_UNCHANGED = -1：废置！
	//	IMREAD_GRAYSCALE = 0：设置此表示，将图像转化为单通道灰度图。
	//	IMREAD_COLOR = 1：将图像转化为三通道BGR彩色图像。
	//	IMREAD_ANYDEPTH = 2：若载入图像的深度为16或32位就返回对应深度的图像，否则将图像转换为8位图像
	//	IMREAD_ANYCOLOR = 4:图像可被读取为任意可能的彩色格式
	//	IMREAD_LOAD_GDAL = 8 : 使用文件格式驱动加载图像
	Mat m1 = imread("d:\\3my.png",2|4);//原图
	Mat m2 = imread("d:\\3my.png", 0);//黑白
	Mat m3 = imread("d:\\mydata\\22.png", 199);//缩小
	namedWindow("m1");
	imshow("m1", m1);
	namedWindow(sTitle);
	imshow(sTitle, m2);
	namedWindow("m3");
	imshow("m3", m3);




	Mat dst;
	//将dst缩小到16*16
	resize(m2, dst, Size(16, 16), 0, 0, CV_INTER_LINEAR);
	//将m2转换为二值图,type选THRESH_BINARY，大于阈值的设置为maxval(255),其它置0  
	threshold(dst, dst, 96, 255, THRESH_BINARY);


	namedWindow("dst");
	imshow("dst", dst);
	cout << dst << endl;
	cout << dst.rows << endl;
	cout << dst.cols << endl;

	namedWindow(dTitle, CV_WINDOW_AUTOSIZE);
	ss = imread("d:\\3my.png", 0);//黑白
	// 滑动条    
	int nThreshold = 0;
	cvCreateTrackbar(barName, sTitle, &nThreshold, 254, on_trackbar);
	//on_trackbar(1);
	threshold(ss, dd, 88, 255, THRESH_BINARY);
	imshow(dTitle, dd);

	cv::waitKey(0);
}