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
	IPv4,
	IPv6
};

enum PResult
{
	P_Success,
	P_NotYetImplemented
};

enum SocketOption
{
	TCP_NoDelay, //TRUE = disable nagle's algorithm
};

typedef SOCKET SocketHandle;
