#ifndef SOCKET_C
#define SOCKET_C
#include "Socket.h"

Socket::Socket(IPVersion ipversion, SocketHandle handle)
	:ipversion(ipversion), handle(handle)
{
	assert(ipversion == IPVersion::IPv4);
}

PResult Socket::Create()
{
	assert(ipversion == IPVersion::IPv4);
	if (handle != INVALID_SOCKET)
	{
		return PResult::P_NotYetImplemented;
	}
	handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //attempt to create socket
	if (handle == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		return PResult::P_NotYetImplemented;
	}
	return PResult::P_Success;
}

PResult Socket::Close()
{
	if (handle == INVALID_SOCKET)
	{
		return PResult::P_NotYetImplemented;
	}
	int result = closesocket(handle);
	if (result != 0) //if error occurred while trying to close socket
	{
		int error = WSAGetLastError();
		return PResult::P_NotYetImplemented;
	}
	handle = INVALID_SOCKET;
	return PResult::P_Success;
}

SocketHandle Socket::GetHandle()
{
	return handle;
}

IPVersion Socket::GetIPVersion()
{
	return ipversion;
}

bool Network::Initialize()
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0) //If winsock API failed to start up
	{
		std::cerr << "Failed to start up the winsock API." << std::endl;
		return false;
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) //If version received does not match version requested (2.2)
	{
		std::cerr << "Could not find a usable version of the winsock api dll." << std::endl;
		return false;
	}

	return true;
}

void Network::Shutdown()
{
	WSACleanup();
}

#endif

int main() {
	return 0;
}