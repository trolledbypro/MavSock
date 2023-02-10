#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>

// Enumerators
enum Vehicle {
	PrimaryAircraft,
	PADA
};

enum IPVersion
{
	Unknown,
	IPv4,
	IPv6
};

enum PResult
{
	P_Success,
	P_GenericError
};

enum SocketOption
{
	TCP_NoDelay, //TRUE = disable nagle's algorithm
};

typedef SOCKET SocketHandle;
const int g_MaxPacketSize = 8192;

