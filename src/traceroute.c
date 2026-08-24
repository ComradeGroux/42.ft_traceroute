#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "request.h"
#include "response.h"
#include "stats.h"

void	print_hop(int hop_num, t_stats *stats)
{
	if (stats->rtt[0] == -42.0 && stats->rtt[1] == -42.0 && stats->rtt[2] == -42.0)
	{
		printf(" %d  *  *  *\n", hop_num);
		return;
	}
	printf(" %d  %s", hop_num, stats->ip);
	for (uint8_t i = 0; i < 3; i++)
	{
		printf("  ");
		if (stats->rtt[i] <= 0.0)
			printf("*");
		else
			printf("%.3f ms", stats->rtt[i]);
	}
	printf("\n");
}

void	traceroute(t_sock *s, char *dst_name)
{
	int		hop_num = 1;
	t_stats	stats = {
		.ip = ""
	};
	uint8_t	keepRunning = 1;
	uint8_t	request[PACKET_LEN];
	uint8_t response[PACKET_LEN];
	memset(request, 0, PACKET_LEN);
	memset(response, 0, PACKET_LEN);

	printf("ft_traceroute to %s (%s), 30 hops max, %d byte packets\n", dst_name, s->ip_str, PACKET_LEN);
	while (keepRunning && hop_num <= 30)
	{
		for (size_t i = 0; i < 3; i++)
		{
			memset(request, 0, PACKET_LEN);
			send_request(s, request, hop_num);
			stats.rtt[i] = receive_response(s, &stats, response, &keepRunning);
		}
		print_hop(hop_num, &stats);

		hop_num++;
	}

	close(s->socket);
}
