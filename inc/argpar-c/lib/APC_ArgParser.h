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

/**
 * @brief Initialize parser with argc and argv from the `main()` function
 * @param argc Number of arguments
 * @param argv Argument values
 * @return Constructed APC_ArgParser with all the recommended data
 */
APC_ArgParser apc_init(int argc, char *argv[]);

/**
 * @brief Initialize information for params
 * * Constructed empty aliases
 * * `required` defaults to `false`
 * @return Initialized APC_ArgInfo with default values
 */
APC_ArgInfo apc_initInfo(void);

/**
 * @brief Convert the default RGB format to a valid RGB struct and parse all extended commands
 * * ${r,g,b}
 * * ${R}
 * * ${BOLD}
 * @return New APC_RGB with RGB data and extended data type (italic, bold, reset, ...)
 */
APC_RGB __apc_rgbToRGBStruct(const char *rgbStr);

/**
 * @brief Free all allocated data for argument parser
 * @param argpar Argument parser to free from memory
 */
void apc_destroy(APC_ArgParser *argpar);

/**
 * @brief Automatically generate help and pass it to the string to return
 * @param argpar Argument parser to generate help from
 * @param title Title of the help
 * @param topInfo Top information (eg: Little intro of the tool)
 * @param lowerInfo Lower information (eg: License, ...)
 * @return Generated help with escape characters for colors (if enabled)
 */
char *apc_generateHelp(APC_ArgParser *argpar,
		const char *title,
		const char *topInfo,
		const char *lowerInfo
);

/**
 * @brief Format color;
 * * Remove color commands if color disabled
 * * Convert to color ANSI escape code if enabled
 * @param argpar Argument to format color and get data from
 * @param msg Message to format
 */
char *__apc_colorFormat(APC_ArgParser *argpar, const char *msg);

/**
 * @brief Convert RGB to ANSI escape code
 * @param r Red color
 * @param g Green color
 * @param b Blue color
 * @return Constructed ANSI escape code
 */
char *__apc_setRGB(int r, int g, int b);

/**
 * @brief Setup Arg Information to the `argpar`
 * @param argpar Argument Parser to modify information from
 * @param info Create information about the param
 * @return false if fail, true if success
 */
bool apc_add(APC_ArgParser *argpar, APC_ArgInfo info);

/**
 * @brief Get if there is an argument present
 * @param argpar Get parameters from argpar
 * @param id ID to fetch
 * @return false if fail, true if success
 */
bool apc_get(APC_ArgParser *argpar, const char *id);

__APC_CPP_GUARD_CLOSE

#undef __APC_CPP_GUARD_OPEN
#undef __APC_CPP_GUARD_CLOSE

#endif  // INCLUDE_LIB_APC_ARGPARSER_H_

