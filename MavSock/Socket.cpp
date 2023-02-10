#include "Socket.h"
#include <assert.h>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#endif

namespace MavSock
{
	Socket::Socket(IPVersion ipversion, SocketHandle handle)
		:ipversion(ipversion), handle(handle)
	{
		assert(ipversion == IPVersion::IPv4);
	}

	MSResult Socket::Create()
	{
		assert(ipversion == IPVersion::IPv4);

		if (handle != INVALID_SOCKET)
		{
			return MSResult::MS_GenericError;
		}

		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //attempt to create socket

		if (handle == INVALID_SOCKET)
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		if (SetBlocking(false) != MSResult::MS_Success)
		{
			return MSResult::MS_Success;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != MSResult::MS_Success)
		{
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::Close()
	{
		if (handle == INVALID_SOCKET)
		{
			return MSResult::MS_GenericError;
		}

		int result = closesocket(handle);
		if (result != 0) //if error occurred while trying to close socket
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		handle = INVALID_SOCKET;
		return MSResult::MS_Success;
	}

	MSResult Socket::Bind(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (result != 0) //if an error occurred
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::Listen(IPEndpoint endpoint, int backlog)
	{
		if (Bind(endpoint) != MSResult::MS_Success)
		{
			return MSResult::MS_GenericError;
		}

		int result = listen(handle, backlog);
		if (result != 0) //If an error occurred
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::Accept(Socket & outSocket)
	{
		sockaddr_in addr = {};
#ifdef _WIN32
		int len = sizeof(sockaddr_in);
#else
		socklen_t len = sizeof(sockaddr_in);
#endif		
		SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
		if (acceptedConnectionHandle == INVALID_SOCKET)
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}
		IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
		std::cout << "New connection accepted!" << std::endl;
		newConnectionEndpoint.Print();
		outSocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);
		return MSResult::MS_Success;
	}

	MSResult Socket::Connect(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (result != 0) //if an error occurred
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}
		return MSResult::MS_Success;
	}

	MSResult Socket::Send(const void * data, int numberOfBytes, int & bytesSent)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR)
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::Recv(void * destination, int numberOfBytes, int & bytesReceived)
	{
		bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);
		
		if (bytesReceived == 0) //If connection was gracefully closed
		{
			return MSResult::MS_GenericError;
		}

		if (bytesReceived == SOCKET_ERROR)
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::SendAll(const void * data, int numberOfBytes)
	{

		int totalBytesSent = 0;

		while (totalBytesSent < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesSent;
			int bytesSent = 0;
			char * bufferOffset = (char*)data + totalBytesSent;
			MSResult result = Send(bufferOffset, bytesRemaining, bytesSent);
			if (result != MSResult::MS_Success)
			{
				return MSResult::MS_GenericError;
			}
			totalBytesSent += bytesSent;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::RecvAll(void * destination, int numberOfBytes)
	{
		int totalBytesReceived = 0;

		while (totalBytesReceived < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesReceived;
			int bytesReceived = 0;
			char * bufferOffset = (char*)destination + totalBytesReceived;
			MSResult result = Recv(bufferOffset, bytesRemaining, bytesReceived);
			if (result != MSResult::MS_Success)
			{
				return MSResult::MS_GenericError;
			}
			totalBytesReceived += bytesReceived;
		}

		return MSResult::MS_Success;
	}

	SocketHandle Socket::GetHandle()
	{
		return handle;
	}

	IPVersion Socket::GetIPVersion()
	{
		return ipversion;
	}

	MSResult Socket::SetBlocking(bool isBlocking)
	{
		unsigned long nonBlocking = 1;
		unsigned long blocking = 0;
		assert(isBlocking == nonBlocking);		// Forcing all sockets to be non-blocking
#ifdef _WIN32
		int result = ioctlsocket(handle, FIONBIO, isBlocking ? &blocking : &nonBlocking);
#else
		int flags = fcntl(fd, F_GETFL, 0);
		if (flags == SOCKET_ERROR) {
			return MSResult::MS_GenericError;
		}
		int result = fcntl(handle, F_SETFL, isBlocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
#endif // _WIN32
		if (result == SOCKET_ERROR)
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}
		return MSResult();
	}

	MSResult Socket::SetSocketOption(SocketOption option, BOOL value)
	{
		int result = 0;
		switch (option)
		{
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
			break;
		default:
			return MSResult::MS_GenericError;
		}

		if (result != 0) //If an error occurred
		{
#ifdef _WIN32
			int error = WSAGetLastError();
#endif
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}
}