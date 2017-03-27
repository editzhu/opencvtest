#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/highgui.hpp"
#ifdef HAVE_OPENCV_OCL
#define _OCL_KNN_ 1 // select whether using ocl::KNN method or not, default is using
#define _OCL_SVM_ 1 // select whether using ocl::svm method or not, default is using
#include "opencv2/ocl/ocl.hpp"
#endif

#include <stdio.h>

using namespace std;
using namespace cv;
using namespace cv::ml;

const Scalar WHITE_COLOR = Scalar(255, 255, 255);
const string winName = "points";
const int testStep = 5;

Mat img, imgDst;
RNG rng;

vector<Point>  trainedPoints;
vector<int>    trainedPointsMarkers;
const int MAX_CLASSES = 2;//classnum 分类器种类的数量，这里只有2类
vector<Vec3b>  classColors(MAX_CLASSES);
int currentClass = 0;
vector<int> classCounters(MAX_CLASSES);

static void on_mouse(int event, int x, int y, int /*flags*/, void*)
{
	if (img.empty())
		return;

	int updateFlag = 0;

	if (event == EVENT_LBUTTONUP)
	{
		trainedPoints.push_back(Point(x, y));
		trainedPointsMarkers.push_back(currentClass);
		classCounters[currentClass]++;
		updateFlag = true;
	}

	//draw
	if (updateFlag)
	{
		img = Scalar::all(0);

		// draw points
		for (size_t i = 0; i < trainedPoints.size(); i++)
		{
			Vec3b c = classColors[trainedPointsMarkers[i]];
			circle(img, trainedPoints[i], 5, Scalar(c), -1);
		}

		imshow(winName, img);
	}
}

static Mat prepare_train_samples(const vector<Point>& pts)
{
	Mat samples;
	Mat(pts).reshape(1, (int)pts.size()).convertTo(samples, CV_32F);//reshape 参数1 通道数，行数，如果是0则保持不变，这里用来将X*y的矩阵变成单行的向量
	return samples;
}

static Ptr<TrainData> prepare_train_data()
{
	Mat samples = prepare_train_samples(trainedPoints);
	return TrainData::create(samples, ROW_SAMPLE, Mat(trainedPointsMarkers));
}

static void predict_and_paint(const Ptr<StatModel>& model, Mat& dst)
{
	Mat testSample(1, 2, CV_32FC1);
	for (int y = 0; y < img.rows; y += testStep)
	{
		for (int x = 0; x < img.cols; x += testStep)
		{
			testSample.at<float>(0) = (float)x;
			testSample.at<float>(1) = (float)y;

			int response = (int)model->predict(testSample);
			dst.at<Vec3b>(y, x) = classColors[response];
		}
	}
}

static void find_decision_boundary_ANN(const Mat&  layer_sizes)
{
	Mat trainClasses = Mat::zeros((int)trainedPoints.size(), (int)classColors.size(), CV_32FC1);
	for (int i = 0; i < trainClasses.rows; i++)
	{
		trainClasses.at<float>(i, trainedPointsMarkers[i]) = 1.f;
	}

	Mat samples = prepare_train_samples(trainedPoints);
	Ptr<TrainData> tdata = TrainData::create(samples, ROW_SAMPLE, trainClasses);

	//show tdata's data
	imshow("BP samples Example", samples);
	Mat samplesUp;
	resize(samples, samplesUp, Size(samples.cols * 10, samples.rows * 10), (0, 0), (0, 0), 3);
	imshow("BP samplesUp Example", samplesUp);

	imshow("BP trainClasses Example", trainClasses);
	resize(trainClasses, samplesUp, Size(trainClasses.cols * 10, trainClasses.rows * 10), (0, 0), (0, 0), 3);
	imshow("BP samplesUp Example", samplesUp);

	Ptr<ANN_MLP> ann = ANN_MLP::create();
	ann->setLayerSizes(layer_sizes);
	ann->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1);
	ann->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 300, FLT_EPSILON));
	ann->setTrainMethod(ANN_MLP::BACKPROP, 0.001);
	ann->train(tdata);
	predict_and_paint(ann, imgDst);
}

int main22()
{
	cout << "Use:" << endl
		<< "  key '0' .. '1' - switch to class #n" << endl
		<< "  left mouse button - to add new point;" << endl
		<< "  key 'r' - to run the ML model;" << endl
		<< "  key 'i' - to init (clear) the data." << endl << endl;

	cv::namedWindow("points", 1);
	img.create(480, 640, CV_8UC3);
	imgDst.create(480, 640, CV_8UC3);

	imshow("points", img);
	setMouseCallback("points", on_mouse);

	classColors[0] = Vec3b(0, 255, 0);
	classColors[1] = Vec3b(0, 0, 255);

	for (;;)
	{
		uchar key = (uchar)waitKey();

		if (key == 27) break;

		if (key == 'i') // init
		{
			img = Scalar::all(0);

			trainedPoints.clear();
			trainedPointsMarkers.clear();
			classCounters.assign(MAX_CLASSES, 0);

			imshow(winName, img);
		}

		if (key == '0' || key == '1')
		{
			currentClass = key - '0';
		}

		if (key == 'r') // run
		{
			double minVal = 0;
			minMaxLoc(classCounters, &minVal, 0, 0, 0);
			if (minVal == 0)
			{
				printf("each class should have at least 1 point\n");
				continue;
			}
			img.copyTo(imgDst);
			Mat layer_sizes1(1, 5, CV_32SC1);
			layer_sizes1.at<int>(0) = 2;
			layer_sizes1.at<int>(1) = 4;
			layer_sizes1.at<int>(2) = 4;
			layer_sizes1.at<int>(3) = 4;
			layer_sizes1.at<int>(4) = 2;
			find_decision_boundary_ANN(layer_sizes1);
			imshow("ANN", imgDst);
		}
	}
	return 0;
}