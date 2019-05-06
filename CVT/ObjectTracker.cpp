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
	trackingHistory.push_back(box);
	return box;
}

bool ObjectTracker::confirmTracker() {
	if (trackingHistory.size() == 100) {
		//compute the average value of the top left x coordinate (chosen arbitrarily)
		//from the last 100 tracker updates
		double sum = 0;
		for (int i = trackingHistory.size(); i > 1; i--) {
			sum += trackingHistory.at(i - 1).x;
		}
		double avg = sum / trackingHistory.size();
		if (std::abs(avg - box.x) < 10) {
			trackingHistory.clear();
			return false;
		}
		trackingHistory.clear();
	}
	return true;
}

ObjectTracker::~ObjectTracker()
{
}