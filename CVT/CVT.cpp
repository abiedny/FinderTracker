#include "pch.h"
#include <iostream>
#include <cstring>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>
#include "ObjectFinder.h"
#include "ObjectTracker.h"

using namespace std;

const cv::Scalar RED = cv::Scalar(0, 0, 255);

int main(int argc, char** argv) {
	//camera stuff
	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "Couldn't open camera.\n";
		return -1;
	}

	//First we start the object finder, then enter finding loop
	ObjectFinder* finder = new ObjectFinder();
	for (;;) {
		cap.read(frame);
		finder->findFacesHOG(frame);
		if (!finder->currentFaces.empty()) {
			//if there's something here, a face has been found! Switch to tracking mode
			ObjectTracker* tracker = new ObjectTracker(frame, finder->currentFaces[0]);
			for (;;) {
				//draw the box on the frame, and then show it, with the delay cuz yeah
				cv::rectangle(frame, tracker->updateBox(frame), RED);
				cv::imshow("feed", frame);
				cap.read(frame);
				if (cv::waitKey(5) >= 0) break;
			}
		}
		cv::imshow("feed", frame);
		if (cv::waitKey(5) >= 0) break;
	}

	return 0;
}