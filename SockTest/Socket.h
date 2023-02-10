#ifndef SOCKET_H
#define SOCKET_H
// Libraries
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "Enums.h"
#include "IPEndpoint.h"
#include <c_library_v2/common/mavlink.h>
#include <c_library_v2/common/mavlink_msg_altitude.h>
#include <c_library_v2/common/mavlink_msg_gps_raw_int.h>
#include <c_library_v2/common/mavlink_msg_global_position_int.h>

//Standard includes
#include <string>
#include <thread>
#include <iostream>
#include <assert.h>
#include <thread>

class Socket
{
public:
	Socket(IPVersion ipversion = IPVersion::IPv4,
		SocketHandle handle = INVALID_SOCKET);
	PResult Create();
	PResult Close();
	PResult Bind(IPEndpoint endpoint);
	PResult Listen(IPEndpoint endpoint, int backlog = 5);
	PResult Accept(Socket& outSocket);
	PResult Connect(IPEndpoint endpoint);
	PResult Send(const void* data, int numberOfBytes, int& bytesSent);
	PResult Recv(void* destination, int numberOfBytes, int& bytesReceived);
	PResult SendAll(const void* data, int numberOfBytes);
	PResult RecvAll(void* destination, int numberOfBytes);
	SocketHandle GetHandle();
	IPVersion GetIPVersion();
private:
	PResult SetSocketOption(SocketOption option, BOOL value);
	IPVersion ipversion = IPVersion::IPv4;
	SocketHandle handle = INVALID_SOCKET;
};

#endif