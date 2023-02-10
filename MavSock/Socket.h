#pragma once
#include "SocketHandle.h"
#include "MSResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Constants.h"

namespace MavSock
{
	class Socket
	{
	public:
		Socket(	IPVersion ipversion = IPVersion::IPv4,
				SocketHandle handle = INVALID_SOCKET);
		MSResult Create();
		MSResult Close();
		MSResult Bind(IPEndpoint endpoint);
		MSResult Listen(IPEndpoint endpoint, int backlog = 5);
		MSResult Accept(Socket & outSocket);
		MSResult Connect(IPEndpoint endpoint);
		MSResult Send(const void * data, int numberOfBytes, int & bytesSent);
		MSResult SendTo(const void* data, int numberOfBytes, int& bytesSent, IPEndpoint endpoint);
		MSResult Recv(void * destination, int numberOfBytes, int & bytesReceived);
		int RecvFrom(void* destination, int numberOfBytes, int& bytesReceived, IPEndpoint endpoint);
		MSResult SendAll(const void * data, int numberOfBytes);
		MSResult RecvAll(void * destination, int numberOfBytes);
		SocketHandle GetHandle();
		IPVersion GetIPVersion();
		MSResult SetBlocking(bool isBlocking);
	private:
		MSResult SetSocketOption(SocketOption option, BOOL value);
		IPVersion ipversion = IPVersion::IPv4;
		SocketHandle handle = INVALID_SOCKET;
	};
}