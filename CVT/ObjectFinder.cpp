#include "pch.h"
#include "ObjectFinder.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

ObjectFinder::ObjectFinder()
{
	cc = cv::CascadeClassifier(cv::String("C:\\opencv\\install\\etc\\haarcascades\\haarcascade_fullbody.xml"));
	hog = cv::HOGDescriptor();
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}

void ObjectFinder::findFaces(cv::Mat inImage) {
	vector<cv::Rect> faces;
	cc.detectMultiScale(inImage, faces, 1.1, 3);
	currentFaces = faces;
}

void ObjectFinder::findFacesHOG(cv::Mat inImage) {
	vector<cv::Rect> faces;
	hog.detectMultiScale(inImage, faces,0.5, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2, false);
	currentFaces = faces;
	adjustRect();
}

void ObjectFinder::adjustRect() {
	if (!currentFaces.empty()) {
		for (int i = 0; i < currentFaces.size(); i++) {
			currentFaces[i].x += cvRound(currentFaces[i].width*0.1);
			currentFaces[i].width = cvRound(currentFaces[i].width*0.25);
			currentFaces[i].y += cvRound(currentFaces[i].height*0.07);
			currentFaces[i].height = cvRound(currentFaces[i].height*0.25);
		}
	}
}

ObjectFinder::~ObjectFinder()
{
	cc.~CascadeClassifier();
	hog.~HOGDescriptor();
}