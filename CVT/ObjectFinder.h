#pragma once
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>
class ObjectFinder
{
public:
	ObjectFinder();
	void findFaces(cv::Mat);
	void findFacesHOG(cv::Mat);
	std::vector<cv::Rect> currentFaces;

	~ObjectFinder();

private:
	void adjustRect();

	cv::CascadeClassifier cc;
	cv::HOGDescriptor hog;
};