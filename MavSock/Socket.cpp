#include "Socket.h"
#include <assert.h>
#include <iostream>

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
			int error = WSAGetLastError();
			return MSResult::MS_GenericError;
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
			int error = WSAGetLastError();
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
			int error = WSAGetLastError();
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
			int error = WSAGetLastError();
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}

	MSResult Socket::Accept(Socket & outSocket)
	{
		sockaddr_in addr = {};
		int len = sizeof(sockaddr_in);
		SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
		if (acceptedConnectionHandle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
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
			int error = WSAGetLastError();
			return MSResult::MS_GenericError;
		}
		return MSResult::MS_Success;
	}

	MSResult Socket::Send(const void * data, int numberOfBytes, int & bytesSent)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
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
			int error = WSAGetLastError();
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
			int error = WSAGetLastError();
			return MSResult::MS_GenericError;
		}

		return MSResult::MS_Success;
	}
}