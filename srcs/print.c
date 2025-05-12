#include "ft_traceroute.h"

void printHop(t_root *data)
{
	if (data->ttl < 10)
		printf(" %d  ", data->ttl);
	else
		printf("%d  ", data->ttl);
	for (unsigned int i = 0; i < data->probes; i++)
	{
		if (i == 0 && !isTimeout(data))
			printf("%s (%s)", data->hostname, data->current_ip[i]);
		else if (ft_strcmp(data->current_ip[i], "VOID") != 0 && isNewIp(data, i))
			printf("%s (%s)", data->current_ip[i], data->current_ip[i]);
		if (data->timeArray[i] == 0 || data->timeArray[i] / 1000.0 > 1000.0)
			printf(" *  ");
		else
			printf("  %.3f ms  ", (data->timeArray[i]) / 1000.0);
	}
	printf("\n");
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

bool isNewIp(t_root *data, unsigned int i)
{
	for (unsigned int j = 0; j < i; j++)
	{
		if (ft_strcmp(data->current_ip[i], data->current_ip[j]) == 0)
			return false;
	}
	return true;
}