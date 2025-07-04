#include "CommunicatorHelper.h"
#include <string>
#include <exception>

using std::exception;

/*
This function reads a 1 byte message code from the socket and converts it to int.
Inputs: A conversation socket to read the message code from.
Output: The converted to int type code.
*/
int CommunicatorHelper::getMsgCodeFromSock(const SOCKET sock)
{
    int reqCode = 0, flags = 0;
    char msgCode[TYPE_CODE_LENGTH] = { 0 };

    if (recv(sock, msgCode, sizeof(msgCode), flags) == INVALID_SOCKET)
    {
        throw exception(("Error while receiving from socket: " + std::to_string(sock)).c_str());
    }

    // Casts the read code into int
    memcpy(&reqCode, &msgCode[0], TYPE_CODE_LENGTH);

    return reqCode;
}

/*
This function reads a 4 bytes data size so it will help to read the data that comes after.
Inputs: A conversation socket to read the size field from.
Output: The converted to int data size.
*/
int CommunicatorHelper::getDataSizeFromSock(const SOCKET sock)
{
    int dataSize = 0, flags = 0;
    char dataSizeFiled[DATA_LENGTH_SIZE] = { 0 };

    if (recv(sock, dataSizeFiled, sizeof(dataSizeFiled), flags) == INVALID_SOCKET)
    {
        throw exception(("Error while receving from socket: " + std::to_string(sock)).c_str());
    }

    // Casts the read code into int
    memcpy(&dataSize, &dataSizeFiled[0], TYPE_CODE_LENGTH);

    return dataSize;
}

/*
This function reads data from the given socket by using the data size passed as a parameter.
Inputs: A conversation socket to read the data field from, and the data size to read according to.
Output:  The converted to vector<byte> data.
*/
vector<byte> CommunicatorHelper::getContentFromSock(const SOCKET sock, const int dataSize)
{
    char* content = new char[dataSize];
    int flags = 0;

    if (recv(sock, content, dataSize, flags) == INVALID_SOCKET)
    {
        throw exception(("Error while receving from socket: " + std::to_string(sock)).c_str());
    }

    // Casts the read data into a byte vector
    vector<byte> buffer(content, content + dataSize);

    delete[] content;

    return buffer;
}
