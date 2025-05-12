#include "ft_traceroute.h"

void freeHop(t_root *data)
{
	for (unsigned int i = 0; i < data->probes; i++)
	{
		if (data->current_ip[i])
			free(data->current_ip[i]);
	}
	free(data->current_ip);
	free(data->timeArray);
	free(data->hostname);
}