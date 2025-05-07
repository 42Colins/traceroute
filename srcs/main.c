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
		doHop(data);
		printHop(data);
		data->ttl++;
		data->starting_port++;
    }
	close(data->sendsocketFd);
    close(data->recvsocketFd);
	return 0;
}
 void doHop(t_root *data)
 {
	data->timeArray = malloc(sizeof(double) * data->probes);
	unsigned int runner = 0;
	struct timeval start_time;
	struct timeval end_time;
	while (runner < data->probes && !data->destReached)
	{
		if (gettimeofday(&start_time, NULL) == -1)
			return ;
		sendPacket(data);
		receivePacket(data);
		gettimeofday(&end_time, NULL);
		data->timeArray[runner] = (((end_time.tv_sec * 1000000) + end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + start_time.tv_usec);
		runner++;
	}
	// free(data->timeArray);
 }