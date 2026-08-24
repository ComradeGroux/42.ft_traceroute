#pragma once

#include "socket.h"
#include "stats.h"

void	send_request(t_sock *s, uint8_t *packet, int ttl);
