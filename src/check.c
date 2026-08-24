#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static inline void	print_help_and_exit(int exit_code)
{
	printf("\
Usage\n\
  ft_traceroute [--help] DESTINATION\n\
\n\
Options:\n\
  -?, --help           Print this help message\n");
	exit(exit_code);
}

void	checking_arguments(int argc, char **argv, char **host)
{
	int	nb_host = 0;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
			print_help_and_exit(EXIT_SUCCESS);\
		else if (strncmp(argv[i], "-", 1) == 0)
		{
			fprintf(stderr, "ft_traceroute: invalid option: %s is not supported\n\n", argv[i]);
			print_help_and_exit(EXIT_FAILURE);
		}
		else
		{
			*host = argv[i];
			nb_host++;
		}
	}

	if (nb_host == 0)
	{
		fprintf(stderr, "ft_traceroute: usage error: Destination address required\n");
		exit(EXIT_FAILURE);
	}
	else if (nb_host > 1)
	{
		fprintf(stderr, "ft_traceroute: usage error: Only one destination address must be provided\n");
		exit(EXIT_FAILURE);
	}
}
