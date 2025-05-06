#include "ft_traceroute.h"

bool isRoot(void)
{
    if (geteuid() == 0)
		return true;
	return false;
}

void sendPacket(t_root *data)
{
	initUdpPacket(data);
	char packet[data->packet_size];
	int packet_size = data->packet_size;
	ft_memset(packet, 0, packet_size);
	setSocketOptions(data->sendsocketFd, data->ttl);
	if (sendto(data->sendsocketFd, packet, packet_size, 0, (struct sockaddr *)&data->send_addr, sizeof(data->send_addr)) <= 0)
	{
		perror("Error while sending packet");
		exit(1);
	}
}

void receivePacket(t_root *data)
{
	ft_memset(&data->recv_addr, 0, sizeof(data->recv_addr));
	char buffer[1024];
	socklen_t addr_len = sizeof(data->recv_addr);

	if (recvfrom(data->recvsocketFd, buffer, sizeof(buffer), 0, (struct sockaddr *)&data->recv_addr, &addr_len) < 0)
	{
		perror("Error while receiving packet");
		exit(1);
	}
	data->current_ip = inet_ntoa(data->recv_addr.sin_addr);
	if (data->current_ip == NULL)
	{
		perror("Error converting IP address");
		exit(1);
	}
	if (ft_strcmp(data->current_ip, data->end_ip) == 0)
	{
		data->destReached = true;
		printf("Destination reached: %s\n", data->current_ip);
		printf("Destination reached: %s\n", data->dest_ip);
	}
	else if (data->ttl >= data->hops_max)
	{
		printf("Max hops reached: %s\n", data->current_ip);
	}
	printf("Received packet from %s\n", data->current_ip);
}

void setSocketOptions(int socketFd, unsigned int ttl)
{
	struct timeval error;
    error.tv_sec = 1;
    error.tv_usec = 0;
	if (setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &error, sizeof(error)) < 0)
	{
		perror("Error setting socket options");
		exit(1);
	}
	if (setsockopt(socketFd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
	{
		perror("Error setting socket options");
		exit(1);
	}
}