//Client Code [Tutorial 17] [Linux Compatible Port]
//Author: Jacob Preston 2019-04-30

#include "IncludeMe.h"
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#define Sleep(duration) usleep(duration*1000)
#endif

using namespace MavSock;

int main()
{
	uint8_t buf[g_MaxPacketSize];
	size_t recsize;
	int bytes_sent;
	mavlink_message_t msg;
	mavlink_status_t status;
	uint16_t len;
	int i = 0;
	int BytesSentorRcv = 0;
	unsigned int temp = 0;
	IPEndpoint dest_addr = IPEndpoint("127.0.0.1", 14550);

	if (Network::Initialize())
	{
		std::cout << "Winsock api successfully initialized." << std::endl;
		Socket socket(IPVersion::IPv4);
		if (socket.Create() == MSResult::MS_Success)
		{
			std::cout << "Socket successfully created." << std::endl;
			if (socket.Bind(IPEndpoint("127.0.0.1", 14551)) == MSResult::MS_Success)
			{
				std::cout << "Successfully connected to server!" << std::endl;
				Sleep(1000);

				while (true)
				{
					MSResult result;

					/*Send Heartbeat */
					mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
					len = mavlink_msg_to_send_buffer(buf, &msg);
					result = socket.SendTo(buf, len, BytesSentorRcv, dest_addr);

					/* Send Status */
					mavlink_msg_sys_status_pack(1, 200, &msg, 0, 0, 0, 500, 11000, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					len = mavlink_msg_to_send_buffer(buf, &msg);
					result = socket.SendTo(buf, len, BytesSentorRcv, dest_addr);

					memset(buf, 0, g_MaxPacketSize);

					bytes_sent = socket.RecvFrom(buf, g_MaxPacketSize, BytesSentorRcv, dest_addr);
					if (bytes_sent == -1)
					{
						result = MSResult::MS_GenericError;
					}

					if (bytes_sent > 0)
					{
						// something received - print out all bytes and parse packet
						mavlink_message_t msg;
						mavlink_status_t status;

						printf("bytes received: %d\ndatagram: ", bytes_sent);
						for (i = 0; i < bytes_sent; ++i)
						{
							temp = buf[i];
							printf("%02x ", (unsigned char)temp);
							if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
							{
								// packet received
								printf("\nreceived packet: sys: %d, comp: %d, len: %d, msg id: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
							}
						}
						printf("\n");
					}

					if (result != MSResult::MS_Success)
						break;

					std::cout << "Attempting to send chunk of data..." << std::endl;
					Sleep(1000);
				}
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
	Network::Shutdown();
	system("pause");
	return 0;
}