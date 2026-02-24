#include <stdio.h>

#include "../inc/argpar-c/lib/APC_ArgParser.h"
#include "../inc/argpar-c/APCpredefines.h"

int main(int argc, char *argv[])
{
	APC_ArgParser parser = apc_init(argc, argv);

	{
		APC_ArgInfo info = apc_initInfo();
		info.id = "version";
		info.param = "--version";
		info.sparam = "-v";
		info.help = "Version information for ArgParser-C";

		apc_add(&parser, info);
	}

	{
		APC_ArgInfo info = apc_initInfo();
		info.id = "help";
		info.param = "--help";
		info.sparam = "-h";
		info.help = "Show this help";

		cvec_push(&info.aliases, const char *, "-?");

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
		const char *licenseMessage =
			"\n\nArgPar-C Copyright (C) 2026  pcannon09\n"
    		"This program comes with ABSOLUTELY NO WARRANTY; for details type `./run.sh --help'.\n"
    		"This is free software, and you are welcome to redistribute it\n"
    		"under certain conditions; type `./run.sh --help' for details.";

		char *helpMsg = apc_generateHelp(
				&parser,
				"APC Help",
				"ArgParser-C help information and demo\nCheck the code for more information",
				licenseMessage);

		printf("%s\n", helpMsg);

		APC_FREE(helpMsg);
	}

	apc_destroy(&parser);

	return 0;
}

