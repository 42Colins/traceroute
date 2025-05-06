#include "ft_traceroute.h"

void initUdpPacket(t_root *data)
{
	ft_memset(&data->send_addr, 0, sizeof(data->send_addr));
	data->send_addr.sin_family = AF_INET;
	data->send_addr.sin_port = htons(data->starting_port);
	data->send_addr.sin_addr.s_addr = 0;
	if (inet_pton(AF_INET, data->dest_ip, &(data->send_addr.sin_addr)) <= 0) {
		perror("Error initializing UDP packets failed");
		exit(1);
	}
	setSocketOptions(data->sendsocketFd, data->ttl);
}