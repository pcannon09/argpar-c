#ifndef INCLUDE_LIB_APC_ARGPARSER_H_
#define INCLUDE_LIB_APC_ARGPARSER_H_

#include <stdbool.h>

#include "cvec/cvec.h"
#include "../APC_config.h"

#ifdef __cplusplus
# 	define __APC_CPP_GUARD_OPEN 		extern "C" {
# 	define __APC_CPP_GUARD_CLOSE 		}
#else
# 	define __APC_CPP_GUARD_OPEN
# 	define __APC_CPP_GUARD_CLOSE
#endif // __cplusplus 

__APC_CPP_GUARD_OPEN

#define __APC_INT2PTR(_int) (char*)(intptr_t)(_int)
#define __APC_PTR2INT(_ptr) (int)(intptr_t)(_ptr)

typedef struct
{
	bool required;

	char *id;
	char *param;
	char *sparam;
	char *help;
} APC_ArgInfo;

typedef struct
{
	bool enableColor;

	int argc;

	char *fullHelp;
	char *helpDescription;

	CVEC args; // CVEC of APC_ArgInfo

	char **argv;
} APC_ArgParser;

APC_ArgParser apc_init(int argc, char *argv[]);

void apc_destroy(APC_ArgParser *argpar);

char *apc_generateHelp(APC_ArgParser *argpar);
char *__apc_colorFormat(APC_ArgParser *argpar, const char *msg);
char *__apc_setRGB(unsigned int r, unsigned int g, unsigned int b);

bool apc_add(APC_ArgParser *argpar, APC_ArgInfo info);
bool apc_get(APC_ArgParser *argpar, const char *id);

__APC_CPP_GUARD_CLOSE

#undef __APC_CPP_GUARD_OPEN
#undef __APC_CPP_GUARD_CLOSE

#endif  // INCLUDE_LIB_APC_ARGPARSER_H_
