#ifndef MAVLINK_SOCKET_H
#define MAVLINK_SOCKET_H
// Libraries
#include <masesk/EasySocket.hpp>
#include <c_library_v2/common/mavlink.h>
#include <c_library_v2/common/mavlink_msg_altitude.h>
#include <c_library_v2/common/mavlink_msg_gps_raw_int.h>
#include <c_library_v2/common/mavlink_msg_global_position_int.h>

//Standard includes
#include <string>
#include <thread>

// Enumerators
enum Vehicle {
	PrimaryAircraft,
	PADA
};

class MavlinkSocket {
public:
	MavlinkSocket(Vehicle, std::string, std::uint16_t);
	MavlinkSocket(const MavlinkSocket&);
	~MavlinkSocket();
	void SendData(std::string);
	std::string EnumToString(Vehicle);
	void ListenServer();
	void KillConnection();
	void CreateListener();

private:
	Vehicle VehicleID;
	std::string VehicleID_string;
	std::string SocketIP;
	std::uint16_t SocketPort;
protected:
	masesk::EasySocket socketmanager;
};

#endif