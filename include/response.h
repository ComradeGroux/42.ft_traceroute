#pragma once

#include "socket.h"
#include "stats.h"

double	receive_response(t_sock *s, t_stats *stats, uint8_t *response, uint8_t *keepRunning);
