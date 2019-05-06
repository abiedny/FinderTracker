#include "pch.h"
#include "ObjectTracker.h"
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

/// <summary>
/// Constructor for the object tracking class. Creates a new Median Flow tracker that is initalized to the parameters
/// passed in.
/// </summary>
/// <param name="initialFrame">The cv::mat of the initial frame the tracker will be initialized on</param>
/// <param name="boundingBox">The bounding box containing the object in the frame to be tracked</param>
ObjectTracker::ObjectTracker(cv::Mat initialFrame, cv::Rect boundingBox)
{
	tracker = cv::TrackerMedianFlow::create(); //definetly use medianflow, its the smoothest
	box = boundingBox;
	tracker->init(initialFrame, box);
}

/// <summary>
/// Method to update the bounding box tracking the object
/// </summary>
/// <param name="inImage">cv::mat of the latest frame, after the tracker has been initialized</param>
/// <returns>A cv::Rect2d that is the updated bounding box tracking the object the tracker was initalized to</returns>
cv::Rect2d ObjectTracker::updateBox(cv::Mat inImage) {
	tracker->update(inImage, box);
	trackingHistory.push_back(box);
	return box;
}

/// <summary>
/// Method used to confirm that the tracker is still accurately tracking a face. Checks the last 100 stored
/// tracking coordinates to make sure that the tracker is not just sitting still. If an object is still being
/// accurately tracked, there will be enough variation in the bounding box for this method to return true.
/// </summary>
/// <returns>bool indicating whether or not the tracker is still accurate. False indicates that the tracker
/// should be destroyed and reinitialized</returns>
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

/// <summary>
/// Class destructor.
/// </summary>
ObjectTracker::~ObjectTracker()
{
}