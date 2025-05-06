#include "ft_traceroute.h"

t_root *initTraceroute(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Invalid input to call the traceroute function\n");
		exit(1);	
	}
	t_root *data;
	data = malloc(sizeof(t_root));
	if (!data)
	{
		printf("malloc failed\n");
		exit(1);
	}
	data->ttl = 1;
	data->dest = argv[1];
	struct hostent *host = gethostbyname(data->dest);
    if (!host) {
        printf("Could not resolve hostname %s\n", data->dest);
        free(data);
        exit(1);
    }
	data->dest_ip = inet_ntoa(*(struct in_addr *)host->h_addr);
	data->end_ip = data->dest_ip;
	data->destReached = false;
	data->hops_max = 30;
	data->packet_size = 60;
	data->starting_port = 33434;
	initRecvSocket(data);
	initSendSocket(data);
	return data;
}

void initSendSocket(t_root *data)
{
	data->sendsocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (data->sendsocketFd < 0)
	{
		perror("Error creating send socket");
		free(data);
		exit (1);
	}
}

void initRecvSocket(t_root *data)
{
	data->recvsocketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->recvsocketFd < 0)
	{
		perror("Error creating receive socket");
		free(data);
		exit (1);
	}
}