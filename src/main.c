#include <stdio.h>

#include "../inc/argpar-c/lib/APC_ArgParser.h"
#include "../inc/argpar-c/APCpredefines.h"

int main(int argc, char *argv[])
{
	APC_ArgParser parser = apc_init(argc, argv);

	{
		APC_ArgInfo info = {0};
		info.id = "version";
		info.param = "--version";
		info.sparam = "-v";
		info.help = "Version information for ArgParser-C";

		apc_add(&parser, info);
	}

	{
		APC_ArgInfo info = {0};
		info.id = "help";
		info.param = "--help";
		info.sparam = "-h";
		info.help = "Show this help";

		apc_add(&parser, info);
	}


	if (apc_get(&parser, "version"))
	{
		printf("== VERSION INFO\n");
		printf("- VERSION: %i.%i.%i\n", 
				APC_VERSION_MAJOR,
				APC_VERSION_MINOR,
				APC_VERSION_PATCH);
		printf("- VERSION STANDARD: %ld\n", APC_VERSION_STD);
		printf("- VERSION STATE: %s\n", APC_VERSION_STATE);
	}

	else if (apc_get(&parser, "help"))
	{
		parser.enableColor = false;

		char *helpMsg = apc_generateHelp(&parser);

		printf("%s\n", helpMsg);

		APC_FREE(helpMsg);
	}

	apc_destroy(&parser);

	return 0;
}

