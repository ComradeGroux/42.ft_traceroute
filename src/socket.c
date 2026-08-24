#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>

#include "socket.h"

static void	resolve_hostname(char* hostname, t_sock* s)
{
	struct addrinfo		hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_ICMP;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	struct addrinfo*	res = NULL;

	int err = getaddrinfo(hostname, NULL, &hints, &res);
	if (err != 0)
	{
		fprintf(stderr, "ft_traceroute error: getaddrinfo error: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}

	if (res == NULL)
	{
		fprintf(stderr, "ft_traceroute error: server error: Host unreachable\n");
		exit(EXIT_FAILURE);
	}

	memcpy(&(s->addr), res->ai_addr, sizeof(struct sockaddr_in));
	freeaddrinfo(res);

	if (inet_ntop(AF_INET, &(s->addr.sin_addr), s->ip_str, INET_ADDRSTRLEN) == NULL)
	{
		perror("ft_traceroute: inet_ntop error");
		exit(EXIT_FAILURE);
	}
}

t_sock	create_socket(char *hostname)
{
	t_sock s;
	memset(&(s.addr), 0, sizeof(s.addr));
	resolve_hostname(hostname, &s);

	s.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (s.socket == -1)
	{
		perror("ft_traceroute: socket error");
		exit(EXIT_FAILURE);
	}

	struct timespec	tv_out = {
		.tv_sec = 1,
		.tv_nsec = 0
	};
	if (setsockopt(s.socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)) < 0)
	{
		close(s.socket);
		perror("ft_traceroute: socket error");
		exit(EXIT_FAILURE);
	}
	int	opt = 1;
	if (setsockopt(s.socket, IPPROTO_IP, IP_RECVERR, &opt, sizeof(opt)) < 0)
	{
		close(s.socket);
		perror("ft_traceroute: socket error");
		exit(EXIT_FAILURE);
	}

	return s;
}
