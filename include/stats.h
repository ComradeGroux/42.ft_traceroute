#pragma once

#include <netinet/ip.h>

typedef struct s_stats {
	char	ip[INET_ADDRSTRLEN];
	double	rtt[3];
} t_stats;
