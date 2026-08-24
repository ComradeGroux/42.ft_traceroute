#include <stdlib.h>

#include "check.h"
#include "socket.h"
#include "traceroute.h"

int	main(int argc, char **argv)
{
	char*		host = NULL;
	checking_arguments(argc, argv, &host);

	t_sock	s = create_socket(host);
	traceroute(&s, host);

	return EXIT_SUCCESS;
}
