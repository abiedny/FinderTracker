#pragma once
#include <Windows.h>
#include <opencv2/core/types.hpp>
class Output
{
public:
	Output();
	bool send(std::string);
	void sendCoords(cv::Rect2d);
	void trimServo(double&, double&);
	~Output();
private:
	HANDLE hSerial;
	DCB dcbSerialParams;
	COMMTIMEOUTS timeouts;
};
