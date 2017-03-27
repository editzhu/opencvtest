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

	//OpenCV���и���������ֱ�ӽ�RGBģ��ת��ΪHSVģ�ͣ�ע�����OpenCV��H��[0, 180���� S ��[0, 255]�� V ��[0, 255]������֪��H���������ܱ�ʾһ���������ɫ������S��V��ȡֵҲҪ��һ����Χ�ڣ���ΪS�������H����ʾ���Ǹ���ɫ�Ͱ�ɫ�Ļ�ϳ̶ȣ�Ҳ��˵SԽС����ɫԽ���ף�Ҳ����Խǳ��V�������H����ʾ���Ǹ���ɫ�ͺ�ɫ�Ļ�ϳ̶ȣ�Ҳ��˵VԽС����ɫԽ���ڡ�����ʵ�飬ʶ����ɫ��ȡֵ�� H��100��140��S��V����90��255֮�䡣һЩ��������ɫH��ȡֵ�����������ã�
	//Orange  0 - 22 Yellow 22 - 38 Green 38 - 75 Blue 75 - 130 Violet 130 - 160 Red 160 - 179
	int iLowH = 20;
	int iHighH = 150;//�ж���ɫ

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
		
		

		//��Ϊ���Ƕ�ȡ���ǲ�ɫͼ��ֱ��ͼ���⻯��Ҫ��HSV�ռ���  
		split(imgHSV, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, imgHSV);
		Mat imgThresholded;
		imshow("1", imgHSV);
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image  

		//������ (ȥ��һЩ���)  
		//Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

		//�ղ��� (����һЩ��ͨ��)  
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