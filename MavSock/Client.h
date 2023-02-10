#pragma once
#include "IncludeMe.h"
#ifndef _WIN32
#include <unistd.h>
#define Sleep(duration) usleep(duration*1000)
#endif

using namespace MavSock;

class Client
{
public:
	bool Connect(IPEndpoint ip);
	bool IsConnected();
	bool Disconnect();
	bool Frame();
private:
	bool isConnected = false;
	Socket socket;
};