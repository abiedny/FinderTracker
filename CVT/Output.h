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
	/// <summary>
	/// WinAPI pointer to the opened serial port.
	/// </summary>
	HANDLE hSerial;
	/// <summary>
	/// Object containing the settings for the serial connection created in this class.
	/// </summary>
	DCB dcbSerialParams;
	/// <summary>
	/// Object containing timout settings for the serial connection.
	/// </summary>
	COMMTIMEOUTS timeouts;
	/// <summary>
	/// Constant byte indicating the camera is in facial detection mode if sent
	/// </summary>
	const char FINDING = 0x00;
	/// <summary>
	/// Constant byte indicating the camera is in tracking mode if sent
	/// </summary>
	const char TRACKING = 0xFF;
};

