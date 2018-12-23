#include "pch.h"
#include "ObjectFinder.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

ObjectFinder::ObjectFinder()
{
	cc = cv::CascadeClassifier(cv::String("C:\\opencv\\install\\etc\\haarcascades\\haarcascade_upperbody.xml"));
}

void ObjectFinder::findFaces(cv::Mat inImage) {
	vector<cv::Rect> faces;
	cc.detectMultiScale(inImage, faces, 1.1, 3);
	currentFaces = faces;
}

ObjectFinder::~ObjectFinder()
{
	cc.~CascadeClassifier();
}