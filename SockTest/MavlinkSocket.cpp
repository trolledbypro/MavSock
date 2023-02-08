#ifndef MAVLINK_SOCKET_C
#define MAVLINK_SOCKET_C
#include "MavlinkSocket.h"
#include <thread>
#endif

MavlinkSocket::MavlinkSocket(Vehicle VID, std::string SIP, std::uint16_t SPort) :
	VehicleID(VID), SocketIP(SIP), SocketPort(SPort), VehicleID_string(EnumToString(VehicleID)) 
{
	socketmanager.socketConnect(EnumToString(VehicleID), SocketIP, SocketPort);
};

MavlinkSocket::MavlinkSocket(const MavlinkSocket& mv)
{
	VehicleID = mv.VehicleID;
	VehicleID_string = mv.VehicleID_string;
	SocketIP = mv.SocketIP;
	SocketPort = mv.SocketPort;
	socketmanager = mv.socketmanager;
}

MavlinkSocket::~MavlinkSocket()
{
	socketmanager.closeConnection(EnumToString(VehicleID));
}

std::string MavlinkSocket::EnumToString(Vehicle veh) {
	switch (veh) {
	case PrimaryAircraft:
		return "PrimaryAircraft";
	default:
		return "PADA";
	}
}

void HandleData(const std::string& data) {
	std::cout << "Client sent: " + data << std::endl;
}

void MavlinkSocket::ListenServer()
{
	std::cout << "Listener thread active!\n";
	socketmanager.socketListen(VehicleID_string, SocketPort, &HandleData);
}

void MavlinkSocket::CreateListener()
{
	std::thread ListenerThread(&MavlinkSocket::ListenServer, this);
	ListenerThread.join();
}

void MavlinkSocket::SendData(std::string data)
{
	socketmanager.socketSend(VehicleID_string, data);
}

void MavlinkSocket::KillConnection()
{
	socketmanager.closeConnection(VehicleID_string);
}