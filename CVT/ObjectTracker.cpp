#include "pch.h"
#include "ObjectTracker.h"
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

ObjectTracker::ObjectTracker(cv::Mat initialFrame, cv::Rect boundingBox)
{
	tracker = cv::TrackerMedianFlow::create(); //definetly use medianflow, its the smoothest
	box = boundingBox;
	tracker->init(initialFrame, box);
}

cv::Rect2d ObjectTracker::updateBox(cv::Mat inImage) {
	tracker->update(inImage, box);
	return box;
}

ObjectTracker::~ObjectTracker()
{
	tracker->~TrackerMedianFlow();
	delete &box;
}