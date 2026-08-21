#include <stdlib.h>

#include "check.h"

int	main(int argc, char **argv)
{
	char*		host = NULL;
	checking_arguments(argc, argv, &host);

	return EXIT_SUCCESS;
}
