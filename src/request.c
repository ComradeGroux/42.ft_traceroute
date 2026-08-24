#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <time.h>

#include "request.h"

static unsigned short	compute_chksum(void *buf, int len)
{
	unsigned short	*w = buf;
	unsigned int	sum = 0;

	for (; len > 1; len -= 2)
		sum += *w++;

	if (len == 1)
		sum += *(unsigned char *)w;

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ~sum;
}

static void	fill_icmp_header(uint8_t *packet, uint16_t sequence)
{
	struct icmphdr	*icmp = (struct icmphdr*)packet;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->un.echo.id = htons(getpid() & 0xFFFF);
	icmp->un.echo.sequence = sequence;
}

static void	fill_payload(uint8_t *packet)
{
	uint8_t			*payload = packet + sizeof(struct icmphdr);
	struct timespec	now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	memcpy(payload, &now, sizeof(now));
}

static void	fill_checksum(uint8_t *packet)
{
	struct icmphdr	*icmp = (struct icmphdr*)packet;
	icmp->checksum = compute_chksum(packet, PACKET_LEN);
}

static void	build_request(t_sock *s, uint8_t *request, int ttl)
{
	fill_icmp_header(request, ttl);
	fill_payload(request);
	fill_checksum(request);
	if (setsockopt(s->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
	{
		close(s->socket);
		perror("ft_traceroute: setsockopt IP_TTL");
		exit(EXIT_FAILURE);
	}
}

void	send_request(t_sock *s, uint8_t *request, int ttl)
{
	build_request(s, request, ttl);
	if (sendto(s->socket, request, PACKET_LEN, 0, (struct sockaddr *)&(s->addr), sizeof(s->addr)) == -1)
	{
		close(s->socket);
		perror("ft_traceroute: send error");
		exit(EXIT_FAILURE);
	}
}
