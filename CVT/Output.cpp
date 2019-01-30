#include "pch.h"
#include "Output.h"
#include <Windows.h>
#include <string>
#include <opencv2/core/types.hpp>

Output::Output()
{
	timeouts = { 0 };
	dcbSerialParams = { 0 };
	LPCWSTR serialPort = L"\\\\.\\COM3";

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
	
	// Set device parameters (38400 baud, 1 start bit, 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		//Couldn't get device state
		CloseHandle(hSerial);
	}
	dcbSerialParams.BaudRate = CBR_115200;
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
	int cx = (int)(br.x + tl.x) / 2;
	int cy = (int)(br.y + tl.y) / 2;

	//then scale it to 0-180 for the servos, with 90 as midpoint
	double fx = (100 / (double)640)*cx + 40;
	double fy = (-80 / (double)480)*cy + 130; 
	trimServo(fx, fy);

	//output to console for debug
	printf("X: %f\n", fx);
	printf("Y: %f\n", fy);

	//format it and send it off
	std::string formatted = "X" + std::to_string((int)fx) + ":Y" + std::to_string((int)fy) + "\n";
	send(formatted);
	return;
}

void Output::trimServo(double& x, double& y)
{
	//tilt 90 points down somewhat
	y += (double)10;
	x += (double)10;
}

Output::~Output()
{
	// Close serial port
	CloseHandle(hSerial);
}