#include <netinet/ip_icmp.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip.h>
#include <linux/errqueue.h>

#include "response.h"

static double	get_elapsed_time(uint8_t *payload)
{
	struct timespec	start;
	memcpy(&start, payload, sizeof(start));

	struct timespec	end;
	clock_gettime(CLOCK_MONOTONIC, &end);

	return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1e6;
}

static void	convert_addr_to_str(t_sock *s, struct sockaddr_in *src, char str[INET_ADDRSTRLEN])
{
	if (inet_ntop(AF_INET, &(src->sin_addr), str, INET_ADDRSTRLEN) == NULL)
	{
		close(s->socket);
		perror("ft_traceroute: inet_ntop error");
		exit(EXIT_FAILURE);
	}
}

static double	receive_icmp_error(t_sock *s, t_stats *stats)
{
	uint8_t				oldPacket[sizeof(struct icmphdr) + sizeof(struct timespec)];
	uint8_t				controlBuff[256];
	struct sockaddr_in	from_addr;
	struct iovec	iov = {
		.iov_base = oldPacket,
		.iov_len = sizeof(oldPacket)
	};
	struct msghdr	messages = {
		.msg_name = &from_addr,
		.msg_namelen = sizeof(from_addr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
		.msg_control = &controlBuff,
		.msg_controllen = sizeof(controlBuff),
		.msg_flags = 0
	};

	ssize_t	n = recvmsg(s->socket, &messages, MSG_ERRQUEUE);
	if (n < 0)
	{
		switch (errno)
		{
			case EWOULDBLOCK:
				return -42.0;
			default:
				perror("ft_traceroute: recvmsg error");
				close(s->socket);
				exit(EXIT_FAILURE);
		}
	}

	struct cmsghdr	*msg = CMSG_FIRSTHDR(&messages);
	while (msg == NULL || msg->cmsg_level != IPPROTO_IP || msg->cmsg_type != IP_RECVERR)
		msg = CMSG_NXTHDR(&messages, msg);
	if (msg == NULL)
		return -2.0;

	struct sock_extended_err	*data = (struct sock_extended_err*)CMSG_DATA(msg);
	if (data->ee_origin != SO_EE_ORIGIN_ICMP || (data->ee_type != ICMP_ECHOREPLY && data->ee_type != ICMP_TIME_EXCEEDED))
		return -2.0;

	convert_addr_to_str(s, (struct sockaddr_in *)SO_EE_OFFENDER(data), stats->ip);

	switch (data->ee_type)
	{
		case ICMP_TIME_EXCEEDED:
			if (n == 8)
				return -1.0;
			uint8_t	*payload = oldPacket + sizeof(struct icmphdr);
			return get_elapsed_time(payload);
		default:
			fprintf(stderr, "ft_traceroute: ICMP error\n");
			return -1.0;
	}
}

double	receive_response(t_sock *s, t_stats *stats, uint8_t *response, uint8_t *keepRunning)
{
	struct sockaddr_in	src;
	socklen_t			src_len = sizeof(src);
	ssize_t	n = recvfrom(s->socket, response, PACKET_LEN, 0, (struct sockaddr*)&src, &src_len);
	if (n < 0)
		return receive_icmp_error(s, stats);

	struct icmphdr	*icmp = (struct icmphdr *)response;
	uint8_t			*payload = response + sizeof(struct icmphdr);
	
	if (icmp->type != ICMP_ECHOREPLY)
		return -1.0;

	*keepRunning = 0;
	convert_addr_to_str(s, &src, stats->ip);

	return get_elapsed_time(payload);
}
