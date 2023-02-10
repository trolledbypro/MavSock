#include "Client.h"
#include <iostream>

bool Client::Connect(IPEndpoint ip)
{
	isConnected = false;
	if (Network::Initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		socket = Socket(ip.GetIPVersion());
		if (socket.Create() == MSResult::MS_Success)
		{
			if (socket.SetBlocking(true) != MSResult::MS_Success)
				return false;


			std::cout << "Socket successfully created." << std::endl;
			if (socket.Connect(ip) == MSResult::MS_Success)
			{
				std::cout << "Successfully connected to server!" << std::endl;
				return true;
			}
			else
			{
				std::cerr << "Failed to connect to server." << std::endl;
			}
			socket.Close();
		}
		else
		{
			std::cerr << "Socket failed to create." << std::endl;
		}
	}
	return false;
}

bool Client::IsConnected()
{
	return isConnected;
}

bool Client::Disconnect()
{
	//TODO
	return false;
}

bool Client::Frame()
{
	MSResult result = MSResult::MS_Success;

// Send some MAVLINK Here
// Receive Mavlink here
	// Refer to UDP Example for the above

	if (result != MSResult::MS_Success)
	{
		isConnected = false;
		return false;
	}

	std::cout << "Attempting to send chunk of data..." << std::endl;
	Sleep(500);
	return true;
}
