#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>  

using namespace cv;
using namespace std;

const char *dTitle = "��ֵͼ";
const char *sTitle = "ԭͼ";
const char *barName = "��ֵ";

Mat ss;
Mat dd;
void on_trackbar(int pos)
{
	//��ͼƬ��ֵ��
	threshold(ss, dd, pos, 255, THRESH_BINARY);
	imshow(dTitle, dd);
}

void main00(void){

	//	IMTEAD_UNCHANGED = -1�����ã�
	//	IMREAD_GRAYSCALE = 0�����ô˱�ʾ����ͼ��ת��Ϊ��ͨ���Ҷ�ͼ��
	//	IMREAD_COLOR = 1����ͼ��ת��Ϊ��ͨ��BGR��ɫͼ��
	//	IMREAD_ANYDEPTH = 2��������ͼ������Ϊ16��32λ�ͷ��ض�Ӧ��ȵ�ͼ�񣬷���ͼ��ת��Ϊ8λͼ��
	//	IMREAD_ANYCOLOR = 4:ͼ��ɱ���ȡΪ������ܵĲ�ɫ��ʽ
	//	IMREAD_LOAD_GDAL = 8 : ʹ���ļ���ʽ��������ͼ��
	Mat m1 = imread("d:\\3my.png",2|4);//ԭͼ
	Mat m2 = imread("d:\\3my.png", 0);//�ڰ�
	Mat m3 = imread("d:\\mydata\\22.png", 199);//��С
	namedWindow("m1");
	imshow("m1", m1);
	namedWindow(sTitle);
	imshow(sTitle, m2);
	namedWindow("m3");
	imshow("m3", m3);




	Mat dst;
	//��dst��С��16*16
	resize(m2, dst, Size(16, 16), 0, 0, CV_INTER_LINEAR);
	//��m2ת��Ϊ��ֵͼ,typeѡTHRESH_BINARY��������ֵ������Ϊmaxval(255),������0  
	threshold(dst, dst, 96, 255, THRESH_BINARY);


	namedWindow("dst");
	imshow("dst", dst);
	cout << dst << endl;
	cout << dst.rows << endl;
	cout << dst.cols << endl;

	namedWindow(dTitle, CV_WINDOW_AUTOSIZE);
	ss = imread("d:\\3my.png", 0);//�ڰ�
	// ������    
	int nThreshold = 0;
	cvCreateTrackbar(barName, sTitle, &nThreshold, 254, on_trackbar);
	//on_trackbar(1);
	threshold(ss, dd, 88, 255, THRESH_BINARY);
	imshow(dTitle, dd);

	cv::waitKey(0);
}