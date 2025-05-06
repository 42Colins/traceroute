#include "ft_traceroute.h"

int main(int argc, char **argv)
{
	if (!isRoot())
	{
		printf("ft_traceroute: you must be root to run this program\n");
		exit(1);
	}
	else if (argc < 2)
	{
		printf("Invalid input to call the traceroute function\n");
		exit(1);	
	}
	t_root *data = initTraceroute(argc, argv);
	printf("Traceroute to %s (%s) with max %u hops\n", data->dest, data->dest_ip, data->hops_max);
    while (!data->destReached)
    {
		printf("Sending packet to %s with TTL %u\n", data->dest, data->ttl);
		sendPacket(data);
		receivePacket(data);
		data->ttl++;
		data->starting_port++;
    }
	close(data->sendsocketFd);
    close(data->recvsocketFd);
	return 0;
}
