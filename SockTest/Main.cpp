#pragma once
#include "MavlinkSocket.h"
#include <iostream>
#include <string>

int main() {
	MavlinkSocket PA = MavlinkSocket(PrimaryAircraft, "127.0.0.1", 14550);
	std::cout << "Created Socket\n";
	PA.CreateListener();
}