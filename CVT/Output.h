#pragma once
#include <Windows.h>
#include <opencv2/core/types.hpp>
class Output
{
public:
	Output();
	bool send(std::string);
	bool send(const char*);
	void sendCoords(cv::Rect2d);
	void sendFindingPacket();
	~Output();
private:
	HANDLE hSerial;
	DCB dcbSerialParams;
	COMMTIMEOUTS timeouts;
	const char FINDING = 0x00;
	const char TRACKING = 0xFF;
};

