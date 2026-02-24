#ifndef INCLUDE_LIB_APC_ARGPARSER_H_
#define INCLUDE_LIB_APC_ARGPARSER_H_

#include <stdbool.h>

#include "cvec/cvec.h"

#ifdef __cplusplus
# 	define __APC_CPP_GUARD_OPEN 		extern "C" {
# 	define __APC_CPP_GUARD_CLOSE 		}
#else
# 	define __APC_CPP_GUARD_OPEN
# 	define __APC_CPP_GUARD_CLOSE
#endif // __cplusplus 

__APC_CPP_GUARD_OPEN

enum APC_RGB_Command
{
	APC_RGB_Command_None,
	APC_RGB_Command_Reset,
	APC_RGB_Command_Bold,
	APC_RGB_Command_Italic,
	APC_RGB_Command_Underline,
};

typedef struct
{
	bool required;

	char *id;
	char *param;
	char *sparam;
	char *help;

	CVEC aliases; // CVEC of: const char*
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

typedef struct
{
	enum APC_RGB_Command __externalAction;

	unsigned int r;
	unsigned int g;
	unsigned int b;
} APC_RGB;

APC_ArgParser apc_init(int argc, char *argv[]);
APC_ArgInfo apc_initInfo(void);
APC_RGB __apc_rgbToRGBStruct(const char *rgbStr);

void apc_destroy(APC_ArgParser *argpar);

char *apc_generateHelp(APC_ArgParser *argpar,
		const char *title,
		const char *topInfo,
		const char *lowerInfo
);
char *__apc_colorFormat(APC_ArgParser *argpar, const char *msg);
char *__apc_setRGB(int r, int g, int b);

bool apc_add(APC_ArgParser *argpar, APC_ArgInfo info);
bool apc_get(APC_ArgParser *argpar, const char *id);

__APC_CPP_GUARD_CLOSE

#undef __APC_CPP_GUARD_OPEN
#undef __APC_CPP_GUARD_CLOSE

#endif  // INCLUDE_LIB_APC_ARGPARSER_H_
