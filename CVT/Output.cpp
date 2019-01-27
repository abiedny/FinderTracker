#include "pch.h"
#include "Output.h"
#include <Windows.h>
#include <string>
#include <opencv2/core/types.hpp>

Output::Output()
{
	timeouts = { 0 };
	dcbSerialParams = { 0 };

	//open a serial port
	hSerial = CreateFile(
		(LPCWSTR)"\\\\.\\COM1",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE) {
		//the serial port failed
		return;
	}
	
	// Set device parameters (38400 baud, 1 start bit, 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		//Couldn't get device state
		CloseHandle(hSerial);
	}
	dcbSerialParams.BaudRate = CBR_38400;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
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

void Output::sendCoords(cv::Rect2d rect)
{
	//first find the center of the rect
	cv::Point2d br = rect.br();
	cv::Point2d tl = rect.tl();
	double cx = (double)(br.x + tl.x) / 2;
	double cy = (double)(br.y + tl.y) / 2;

	//format it and send it off
	send(std::to_string(cx) + "," + std::to_string(cy) + "\n");
	return;
}

Output::~Output()
{
	// Close serial port
	CloseHandle(hSerial);
}
