#ifndef SOCKET_H
#define SOCKET_H
// Libraries
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
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

typedef SOCKET SocketHandle;

class Socket {
public:
	Socket(IPVersion ipversion = IPVersion::IPv4,
		SocketHandle handle = INVALID_SOCKET);
	PResult Create();
	PResult Close();
	SocketHandle GetHandle();
	IPVersion GetIPVersion();
private:
	Vehicle VehicleID;
	IPVersion ipversion = IPVersion::IPv4;
	SocketHandle handle = INVALID_SOCKET;
};

class Network
{
public:
	static bool Initialize();
	static void Shutdown();
};

#endif