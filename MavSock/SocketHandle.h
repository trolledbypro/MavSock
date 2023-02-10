#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>

namespace MavSock
{
#ifdef _WIN32
	typedef SOCKET SocketHandle;
#else
	typedef int SocketHandle;
#endif
}