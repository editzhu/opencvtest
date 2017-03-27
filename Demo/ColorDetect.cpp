#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>  

using namespace cv;
using namespace std;

int maincc()
{
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}
	Mat frame;

	//OpenCV下有个函数可以直接将RGB模型转换为HSV模型，注意的是OpenCV中H∈[0, 180）， S ∈[0, 255]， V ∈[0, 255]。我们知道H分量基本能表示一个物体的颜色，但是S和V的取值也要在一定范围内，因为S代表的是H所表示的那个颜色和白色的混合程度，也就说S越小，颜色越发白，也就是越浅；V代表的是H所表示的那个颜色和黑色的混合程度，也就说V越小，颜色越发黑。经过实验，识别蓝色的取值是 H在100到140，S和V都在90到255之间。一些基本的颜色H的取值可以如下设置：
	//Orange  0 - 22 Yellow 22 - 38 Green 38 - 75 Blue 75 - 130 Violet 130 - 160 Red 160 - 179
	int iLowH = 20;
	int iHighH = 150;//判断绿色

	int iLowS = 90;
	int iHighS = 255;

	int iLowV = 90;
	int iHighV = 255;

	//Create trackbars in "Control" window  
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)  
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)  
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)  
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	while (1)
	{
		cap >> frame;
		imshow("Original", frame); //show the original image 
		Mat imgHSV;
		vector<Mat> hsvSplit;
		cvtColor(frame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV  
		
		

		//因为我们读取的是彩色图，直方图均衡化需要在HSV空间做  
		split(imgHSV, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, imgHSV);
		Mat imgThresholded;
		imshow("1", imgHSV);
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image  

		//开操作 (去除一些噪点)  
		//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

		//闭操作 (连接一些连通域)  
		//morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

		imshow("Thresholded Image", imgThresholded); //show the thresholded image  
	 

		if (waitKey(33) >= 0)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
			
	}
	return 0;
}