#include <stdio.h>
#include "cli.h"

int main(int argc, const char** argv)
{
	if (argc == 1)
	{
		cli_help(argv[0]);
		return 0;
	}
	
	return cli_run(argc, argv);
}
