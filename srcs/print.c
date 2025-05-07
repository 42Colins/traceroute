#include "ft_traceroute.h"

void printHop(t_root *data)
{
	if (!isTimeout(data))	
		printf("%d %s (%s)", data->ttl, data->hostname, data->current_ip);
	else
		printf("%d", data->ttl);
	for (unsigned int i = 0; i < data->probes; i++)
	{
		if (data->timeArray[i] == 0 || data->timeArray[i] / 1000.0 > 1000.0)
			printf(" *");
		else
			printf(" %.3f ms", (data->timeArray[i]) / 1000.0);
	}
	printf("\n");
	free(data->timeArray);
}

bool isTimeout(t_root *data)
{
	for (unsigned int i = 0; i < data->probes; i++)
	{
		if (data->timeArray[i] != 0 && data->timeArray[i] / 1000.0 < 1000.0)
			return false;
	}
	return true;
}