#pragma once

#include "common.h"


std::string GetLastErrorString()
{
	DWORD errorID = GetLastError();
	if (errorID == 0)
		return std::string();

	LPSTR messagebuffer = nullptr;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messagebuffer, 0, NULL);
	std::string message(messagebuffer, size);
	LocalFree(messagebuffer);
	return message;
}