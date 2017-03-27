#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <opencv2/opencv.hpp>
#include <iostream>  
#include <string>  

using namespace std;
using namespace cv;
using namespace ml;

#define SQUARE_ 400//�����α߳��ķ�Χ
#define DATA_ 400//��������
#define features_count 5

int main()
{
	//Setup the BPNetwork  
	// Set up training data  
	//float labels[10] = { 1, 1, 1, 1, 1, 1, -1, -1, -1, -1 };//׼�����࣬��2��
	//Mat labelsMat(10, 2, CV_32FC1, labels);
	//cout << labelsMat;
	//float trainingData[10][2] = { { 11, 12 }, { 21, 22 }, { 44, 32 }, { 71, 42 }, { 141, 162 }, { 111, 111 }, { 441, 412 }, { 311, 312 }, { 222, 333 }, { 333, 212 } };
	//������������ͷ���
	//float labels[DATA_][3];
	vector<int> labels;
	float trainingData[DATA_][features_count];
	//for (int i = 0; i < DATA_; i++){//�����Ϻ�����2��ɫ��
	//	if (i < DATA_ *.5){
	//		trainingData[i][0] = rand() % SQUARE_;
	//		trainingData[i][1] = rand() % int(SQUARE_*.75 - .5*trainingData[i][0]);
	//		trainingData[i][2] = trainingData[i][1] * trainingData[i][0];
	//		labels.push_back(0);
	//	}
	//	else{
	//		trainingData[i][0] = rand() % SQUARE_;
	//		trainingData[i][1] = rand() % (SQUARE_ - int(SQUARE_*.75 - .5*trainingData[i][0])) + SQUARE_*.75 - .5*trainingData[i][0];
	//		trainingData[i][2] = trainingData[i][1] * trainingData[i][0];
	//		labels.push_back(1);
	//	}
	//}	
	for (int i = 0; i < DATA_; i++){//4ɫ�飬������Ҫ35552��382����ʶ��
		if (i < DATA_ *.5){
			trainingData[i][0] = rand() % SQUARE_ *.999 + SQUARE_ *0.001;
			if (trainingData[i][0] < DATA_ *.5)
				trainingData[i][1] = rand() % SQUARE_ *.4 + SQUARE_ *.1;
			else
				trainingData[i][1] = rand() % SQUARE_ *.4 + SQUARE_ *.6;
			trainingData[i][2] = trainingData[i][0] * trainingData[i][0];
			trainingData[i][3] = trainingData[i][1] * trainingData[i][1];
			trainingData[i][4] = trainingData[i][0] * trainingData[i][1];
			labels.push_back(0);
		}
		else{
			trainingData[i][0] = rand() % SQUARE_ *.999 + SQUARE_ *.001;
			if (trainingData[i][0] > DATA_*.5)
				trainingData[i][1] = rand() % SQUARE_ *.4 + SQUARE_ *.1;
			else
				trainingData[i][1] = rand() % SQUARE_ *.4 + SQUARE_ *.6;
			trainingData[i][2] = trainingData[i][0] * trainingData[i][0];
			trainingData[i][3] = trainingData[i][1] * trainingData[i][1];
			trainingData[i][4] = trainingData[i][0] * trainingData[i][1];
			labels.push_back(1);
		}
	}
	//for (int i = 0; i < DATA_; i++){//ͬ��Բ��xy-382�޷�ʶ��.����������x*x,y*y,��ʶ��
	//	trainingData[i][0] = rand() % SQUARE_ *.999 + SQUARE_ *0.001;
	//	trainingData[i][1] = rand() % SQUARE_ *.999 + SQUARE_ *0.001;
	//	trainingData[i][2] = trainingData[i][0] * trainingData[i][0];
	//	trainingData[i][3] = trainingData[i][1] * trainingData[i][1];
	//	if ((trainingData[i][0]<DATA_ *.7&&trainingData[i][0]>DATA_*.3) && (trainingData[i][1]<DATA_ *.7&&trainingData[i][1]>DATA_*.3)){
	//		labels.push_back(0);
	//	}
	//	else{
	//		labels.push_back(1);
	//	}
	//}

	//Mat labelsMat(DATA_, 3, CV_32FC1, labels);
	Mat labelsMat = Mat::zeros((int)labels.size(), 2, CV_32FC1);
	cout << labels.size() << endl;
	for (int i = 0; i < labelsMat.rows; i++){
		//cout << i << ":" << labels[i]<< endl;
		labelsMat.at<float>(i, labels[i]) = 1.f;
		//if (labels[i] == 1){
		//	labelsMat.at<float>(i, 0) = 1.f;
		//	labelsMat.at<float>(i, 1) = 1.f;
		//}
	}
	cout << "labelsMat " << labelsMat << endl;
	Mat trainingDataMat(DATA_, features_count, CV_32FC1, trainingData);//׼��ѵ�����ݣ���10��
	//cout << labelsMat;
	cout << trainingDataMat;


	Mat layerSizes = (Mat_<int>(1, 3) << features_count, 8, 2); //5�㣺����㣬3�����ز������㣬ÿ���Ϊ����perceptron����һ������һ�������2

	Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	// Set up BPNetwork's parameters  
	Ptr<ANN_MLP> ann = ANN_MLP::create();
	ann->setLayerSizes(layerSizes);
	ann->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1);//CvANN_MLP::SIGMOID_SYM ,ѡ��sigmoid��Ϊ��������
	ann->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 300, FLT_EPSILON));
	ann->setTrainMethod(ANN_MLP::BACKPROP, 0.001); //(Back Propagation,BP)���򴫲��㷨
	//imshow("BP trainingDataMat Example", trainingDataMat);
	//imshow("BP labelsMat Example", labelsMat);
	//cout << trainingDataMat << endl;

	ann->train(tData);//ѵ��

	// Data for visual representation  
	int width = SQUARE_, height = SQUARE_;
	Mat image = Mat::zeros(width, height, CV_8UC3);
	Vec3b green(0, 255, 0), blue(255, 0, 0), gray(166, 166, 166);
	// Show the decision regions
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			Mat sampleMat = (Mat_<float>(1, features_count) << i, j, i*i, j*j,i*j);
			//	Mat responseMat;
			//	ann->predict(sampleMat, responseMat);
			//	float* p = responseMat.ptr<float>(0);
			//	if (p[0] > p[1])
			//	{
			//		image.at<Vec3b>(j, i) = green;
			//	}
			//	else
			//	{
			//		image.at<Vec3b>(j, i) = blue;
			//	}
			Mat responseMat(1, 3, CV_32FC1);//����ĵڶ��������Զ����2....

			ann->predict(sampleMat, responseMat);
			//float val = responseMat.at<float>()
			float* p = responseMat.ptr<float>(0);
			if (p[0] - p[1] > 0.1)
			{
				image.at<Vec3b>(j, i) = green;
			}
			else if (p[0] - p[1] < -0.1)
			{
				image.at<Vec3b>(j, i) = blue;
			}
			else{
				int a = -500*abs(p[0] - p[1])+200;
				//cout << a << endl;
				Vec3b gray1 = (a, a, a);

				image.at<Vec3b>(j, i) = gray1;
			}

			//if (j == i)cout << responseMat<<endl;
			//cout  << i << ":" << j << ":" << sampleMat << ":" << responseMat<<endl;
		}

	}
	// Show the training data  
	int thickness = -1;
	int lineType = 8;
	for (int i = 0; i < DATA_; i++){
		if (labels[i] == 1)
			circle(image, Point(trainingData[i][0], trainingData[i][1]), 5, Scalar(0, 0, 0), thickness, lineType);
		else
			circle(image, Point(trainingData[i][0], trainingData[i][1]), 5, Scalar(255, 255, 255), thickness, lineType);

	}

	imwrite("result.png", image);        // save the image   

	imshow("BP Simple Example", image); // show it to the user  
	waitKey(0);

	return 0;
}