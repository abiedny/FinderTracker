#include "pch.h"
#include <iostream>
#include <cstring>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>

using namespace std;

cv::CascadeClassifier cc;

cv::Ptr<cv::TrackerKCF> trackerInit(cv::Mat initialFrame, cv::Rect &boundingBox);
vector<cv::Rect> findFace(cv::Mat);

int main(int argc, char** argv) {
	//first we need to get a video input streaming from the camera
	//inits
	cv::Mat frame;
	cv::VideoCapture cap;

	//Opening camera
	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "Couldn't open camera.\n";
		return -1;
	}
	//grab initial frame and init/setup, then go into tracking/display loop
	//also declare the bounding rectangle and pass by reference
	cap.read(frame);
	cv::Rect knownBox;
	cv::Rect2d box = cv::Rect();
	cv::Mat trackingImage = cv::Mat();

	for (;;)
	{
		// wait for a new frame from camera and store it into frame
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		//and then check if there's a face detected. If there is, create a tracker
		vector<cv::Rect> allFaces;
		allFaces = findFace(frame);
		if (!allFaces.empty()) {
			knownBox = allFaces[0];
			cv::Ptr<cv::TrackerKCF> tracker = trackerInit(frame, knownBox);
			for (;;) {
				//update the bounding box
				tracker->update(frame, box);
				//then draw the bounding box
				frame.copyTo(trackingImage);
				cv::rectangle(trackingImage, box, cv::Scalar(255, 0, 0));
				imshow("Live, Tracking", trackingImage);
				imshow("Live, Raw", frame);
				if (cv::waitKey(5) >= 0) break;
				cap.read(frame);
			}
		}

		// show live if no detection
		imshow("Live, Raw", frame);
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

cv::Ptr<cv::TrackerKCF> trackerInit(cv::Mat initialFrame, cv::Rect &boundingBox) {
	cv::Ptr<cv::TrackerKCF> tracker = cv::TrackerKCF::create();
	tracker->init(initialFrame, boundingBox);
	return tracker;
}

vector<cv::Rect> findFace(cv::Mat inImage) {
	cc = cv::CascadeClassifier(cv::String("C:\\opencv\\install\\etc\\haarcascades\\haarcascade_frontalface_default.xml"));
	vector<cv::Rect> faces;
	cc.detectMultiScale(inImage, faces, 1.1, 6);
	return faces;
}