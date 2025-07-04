#pragma once

#include <vector>
#include <cstring>
#include <WinSock2.h>
#include <Windows.h>

using std::vector;

#define TYPE_CODE_LENGTH 1
#define DATA_LENGTH_SIZE sizeof(int)

/* A static helper class to get the msgCode and data size and content of the client's request */
class CommunicatorHelper
{
public:
	static int getMsgCodeFromSock(const SOCKET sock);
	static int getDataSizeFromSock(const SOCKET sock);
	static vector<byte> getContentFromSock(const SOCKET sock, const int dataSize);
};
