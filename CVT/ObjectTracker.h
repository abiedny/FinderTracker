#pragma once
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>
class ObjectTracker
{
public:
	ObjectTracker(cv::Mat, cv::Rect);
	cv::Rect2d updateBox(cv::Mat);
	bool confirmTracker();

	~ObjectTracker();
private:
	cv::Ptr<cv::TrackerMedianFlow> tracker;
	cv::Rect2d box;
	std::vector<cv::Rect2d> trackingHistory;
};

