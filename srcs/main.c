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
    while (!data->destReached && data->ttl <= data->hops_max)
    {
		data->runner = 0;
		doHop(data);
		printHop(data);
		data->ttl++;
		data->starting_port++;
		freeHop(data);
    }
	close(data->sendsocketFd);
    close(data->recvsocketFd);
	free(data->dest_ip);
	free(data);
	return 0;
}
 void doHop(t_root *data)
 {
	data->timeArray = malloc(sizeof(double) * data->probes);
	data->current_ip = malloc(sizeof(char *) * data->probes);
	if (!data->timeArray || !data->current_ip || (!data->timeArray && !data->current_ip))
	{
		perror("Memory allocation failed");
		exit(1);
	}
	data->hostname = ft_strdup("void");
	if (!data->hostname)
	{
		perror("Memory allocation failed");
		exit(1);
	}
	initCurrentIp(data);
	struct timeval start_time;
	struct timeval end_time;
	while (data->runner < data->probes)
	{
		if (gettimeofday(&start_time, NULL) == -1)
			return ;
		sendPacket(data);
		receivePacket(data);
		gettimeofday(&end_time, NULL);
		data->timeArray[data->runner] = (((end_time.tv_sec * 1000000) + end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + start_time.tv_usec);
		data->runner++;
		data->starting_port++;
	}
}

void initCurrentIp(t_root *data)
{
	for (unsigned int i = 0; i < data->probes; i++)
	{
		data->current_ip[i] = ft_strdup("VOID");
		if (!data->current_ip[i])
		{
			perror("Memory allocation failed");
			exit(1);
		}	
	}
}