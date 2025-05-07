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
	char *temp_ip = inet_ntoa(*(struct in_addr *)host->h_addr);
	data->dest_ip = ft_strdup(temp_ip);
	if (!data->dest_ip) {
		perror("Memory allocation failed");
		exit(1);
	}
	data->probes = 3;
	data->destReached = false;
	data->hops_max = 30;
	data->packet_size = 60;
	data->starting_port = 33434;
    // ft_memset(data->timeArray, 0, sizeof(data->timeArray));
	initRecvSocket(data);
	initSendSocket(data);
	return data;
}

void initSendSocket(t_root *data)
{
    // Create UDP socket
    data->sendsocketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (data->sendsocketFd < 0) {
        perror("Error creating send socket");
        free(data);
        exit(1);
    }
    
    // Set up the destination
    ft_memset(&data->send_addr, 0, sizeof(data->send_addr));
    data->send_addr.sin_family = AF_INET;
    
    // Convert destination IP string to binary form
    if (inet_pton(AF_INET, data->dest_ip, &data->send_addr.sin_addr) <= 0) {
        perror("Invalid destination IP address");
        free(data);
        exit(1);
    }
}

void initRecvSocket(t_root *data)
{
    // Create raw ICMP socket
    data->recvsocketFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (data->recvsocketFd < 0) {
        perror("Error creating receive socket");
        free(data);
        exit(1);
    }
    
    // Set receive socket options
    int on = 1;
    if (setsockopt(data->recvsocketFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("Error setting SO_REUSEADDR");
        // Not fatal, continue
    }
    
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(data->recvsocketFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting socket timeout");
        exit(1);
    }
    struct sockaddr_in recv_addr;
    ft_memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = 0;
    
    if (bind(data->recvsocketFd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("Error binding recv socket");
        exit(1);
    }
}