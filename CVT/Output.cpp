#include "pch.h"
#include "Output.h"
#include <Windows.h>
#include <string>
#include <opencv2/core/types.hpp>

Output::Output()
{
	timeouts = { 0 };
	dcbSerialParams = { 0 };
	LPCWSTR serialPort = L"\\\\.\\COM8";

	//open a serial port
	hSerial = CreateFile(
		serialPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hSerial == INVALID_HANDLE_VALUE) {
		//the serial port failed
		return;
	}
	
	// Set device parameters (baud, start, stop, parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		//Couldn't get device state
		CloseHandle(hSerial);
	}
	dcbSerialParams.BaudRate = CBR_38400;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = PARITY_NONE;
	if (SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		//Couldn't set device parameters
		CloseHandle(hSerial);
	}

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		//Couldn't set timeouts
		CloseHandle(hSerial);
	}
}

//string overload
bool Output::send(std::string sender)
{
	const char* bytes = sender.c_str();

	DWORD bytes_written, total_bytes_written = 0;
	if (!WriteFile(hSerial, bytes, sender.size(), &bytes_written, NULL))
	{
		//Error
		CloseHandle(hSerial);
		return false;
	}
	return true;
}

//single character overload
bool Output::send(const char* toSend) {
	DWORD bytes_written, total_bytes_written = 0;
	if (!WriteFile(hSerial, toSend, 1, &bytes_written, NULL))
	{
		//Error
		CloseHandle(hSerial);
		return false;
	}
	return true;
}

void Output::sendCoords(cv::Rect2d rect)
{
	//first find the center of the rect
	cv::Point2d br = rect.br();
	cv::Point2d tl = rect.tl();
	int cx = (int)(br.x + tl.x) / 2;
	int cy = (int)(br.y + tl.y) / 2;

	//midpoint is x,y 320,240
	double fx = (((320 - (double)cx)*-1)/320)*100;
	double fy = (((240 - (double)cy)*1)/240)*100;

	//output to console for debug
	printf("X: %f\n", fx);
	printf("Y: %f\n", fy);

	char xPacket = (char)fx;
	char yPacket = (char)fy;

	send(&xPacket);
	send(&yPacket);
	send(&TRACKING);

	return;
}

void Output::sendFindingPacket() {
	//first two packets don't matter if the last packet is a finding mode packet
	send(&FINDING);
	send(&FINDING);
	send(&FINDING);
}

Output::~Output()
{
	// Close serial port
	CloseHandle(hSerial);
}
