#include "pch.h"
#include "ObjectFinder.h"
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui.hpp>

/// <summary>
/// Constructor for the object finder class. Creates and initializes a new cascade classifier and 
/// hog descriptor to detect people.
/// </summary>
ObjectFinder::ObjectFinder()
{
	cc = cv::CascadeClassifier(cv::String("C:\\opencv\\install\\etc\\haarcascades\\haarcascade_frontalface_default.xml"));
	hog = cv::HOGDescriptor();
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}

/// <summary>
/// Method to find faces in a given frame and store them in the 
/// currentFaces vector. Empty vector indicates no faces were found.
/// </summary>
/// <param name="inImage">The cv::mat of the frame to detect faces in</param>
void ObjectFinder::findFaces(cv::Mat inImage) {
	std::vector<cv::Rect> faces;
	cc.detectMultiScale(inImage, faces, 1.3, 3);
	currentFaces = faces;
}

/// <summary>
/// Method to find faces in a given frame and store them in the 
/// currentFaces vector. Empty vector indicates no faces were found.
/// This method uses a HOG descriptor, which seems to be less effective than
/// a cascade classifier.
/// </summary>
/// <param name="inImage">The cv::mat of the frame to detect faces in</param>
void ObjectFinder::findFacesHOG(cv::Mat inImage) {
	std::vector<cv::Rect> faces;
	hog.detectMultiScale(inImage, faces,0.5, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2, false);
	currentFaces = faces;
	adjustRect();
}

/// <summary>
/// Method to adjust the dimensions of the bounding boxes of detected faces. Currently unused.
/// </summary>
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

/// <summary>
/// Class destrutor.
/// </summary>
ObjectFinder::~ObjectFinder()
{
}