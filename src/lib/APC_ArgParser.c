#define __USE_MISC
#include "../../inc/argpar-c/lib/APC_ArgParser.h"

#include "../../inc/argpar-c/APCpredefines.h"

#include "cstr/cstr.h"

#include <unistd.h>
#include <stdlib.h>

APC_ArgParser apc_init(int argc, char *argv[])
{
	APC_ArgParser parser = {0};

	parser.argc = argc;
	parser.argv = argv;
	parser.args = cvec_init(-1, sizeof(APC_ArgInfo));
	parser.enableColor = true;

	return parser;
}

void apc_destroy(APC_ArgParser *argpar)
{
	cvec_destroy(&argpar->args);
}

bool apc_add(APC_ArgParser *argpar, APC_ArgInfo info)
{
	// Make addition checks
	if (info.id[0] == '\0' ||
			info.param[0] == '\0')
		return false;

	// If passed, push it:
	cvec_push(&argpar->args, APC_ArgInfo, info);

	return true;
}

bool apc_get(APC_ArgParser *argpar, const char *id)
{
	if (id[0] == '\0'  || !id ||
			!argpar)
		return false;

	// Check main CVEC ARGS
	for (size_t arg = 0 ; arg < argpar->args.size ; arg++)
	{
		APC_ArgInfo *info = cvec_get(&argpar->args, arg);

		if (!info) continue;

		if (strcmp(info->id, id) != 0)
			continue;

		// Check ARGV from apc_init() function
		for (int argv = 0 ; argv < argpar->argc ; argv++)
		{
			if (strcmp(argpar->argv[argv], info->param) == 0 ||
				strcmp(argpar->argv[argv], info->sparam) == 0)
				return true;
		}
	}

	return false;
}

char *__apc_setRGB(unsigned int r, unsigned int g, unsigned int b)
{
	CSTR formatted = cstr_init();

	cstr_set(&formatted, "\033[38;2;");
	cstr_add(&formatted, __APC_INT2PTR(r)); cstr_add(&formatted, ";");
	cstr_add(&formatted, __APC_INT2PTR(g)); cstr_add(&formatted, ";");
	cstr_add(&formatted, __APC_INT2PTR(b)); cstr_add(&formatted, "m");

	char *ret = CSTR_sys_strdup(formatted.data);

	cstr_destroy(&formatted);

	return ret;
}

char *__apc_colorFormat(APC_ArgParser *argpar, const char *msg)
{
    CSTR result = cstr_init();
    cstr_set(&result, msg);

    if (!argpar->enableColor)
    {
        int pos = 0;

        while (1)
        {
            const int start = cstr_findFrom(&result, "${", pos);

            if (start == CSTR_NPOS)
                break;

            const int end = cstr_findFrom(&result, "}", start + 2);

            if (end == CSTR_NPOS)
                break;

            int len = (end - start) + 1;

            cstr_erase(&result, start, len);

            pos = start;
        }
    }

    char *ret = CSTR_sys_strdup(result.data);

    cstr_destroy(&result);

    return ret;
}

char *apc_generateHelp(APC_ArgParser *argpar)
{
	CSTR docs = cstr_init();
	cstr_set(&docs, "");

	for (size_t i = 0 ; i < argpar->args.size ; i++)
	{
		char *toFormat = "Hello ${testest} world ${hey}, bye";
		char *result = __apc_colorFormat(argpar, toFormat);

		printf("RESULT: %s\n", result);

		APC_FREE(result);
	}

	char *data = CSTR_sys_strdup(docs.data);

	cstr_destroy(&docs);

	return data;
}

