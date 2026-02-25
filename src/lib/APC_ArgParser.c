#define __USE_MISC
#include "../../inc/argpar-c/lib/APC_ArgParser.h"

#include "../../inc/argpar-c/APCpredefines.h"
#include "../../inc/argpar-c/APC_config.h"

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

APC_ArgInfo apc_initInfo(void)
{
	APC_ArgInfo info = {0};
	info.aliases = cvec_init(-1, sizeof(const char*));
	info.required = false;

	return info;
}

void apc_destroy(APC_ArgParser *argpar)
{
	for (size_t i = 0 ; i < argpar->args.size ; i++)
	{
		APC_ArgInfo *info = cvec_get(&argpar->args, i);

		if (!info) continue;

		// Destroy
		cvec_destroy(&info->aliases);
	}

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

			// Parse aliases
			for (size_t a = 0 ; a < info->aliases.size ; a++)
			{
				char **alias = cvec_get(&info->aliases, a);

				if (!alias && !*alias)
					continue;

				if (strcmp(argpar->argv[argv], *alias) == 0)
					return true;
			}
		}
	}

	return false;
}

char *__apc_setRGB(int r, int g, int b)
{
	if (r < 0 || r > 255) return NULL;
	if (g < 0 || g > 255) return NULL;
	if (b < 0 || b > 255) return NULL;

	CSTR formatted = cstr_init();

	char tmp[16];

	cstr_set(&formatted, "\033[38;2;");

	snprintf(tmp, sizeof(tmp), "%u", r);
	cstr_add(&formatted, tmp);
	cstr_add(&formatted, ";");

	snprintf(tmp, sizeof(tmp), "%u", g);
	cstr_add(&formatted, tmp);
	cstr_add(&formatted, ";");

	snprintf(tmp, sizeof(tmp), "%u", b);
	cstr_add(&formatted, tmp);
	cstr_add(&formatted, "m");

	char *ret = CSTR_sys_strdup(formatted.data);

	cstr_destroy(&formatted);

	return ret;
}

APC_RGB __apc_rgbToRGBStruct(const char *rgbStr)
{
	APC_RGB rgb = {0};
 	rgb.__externalAction = APC_RGB_Command_None;

	// Reset terminal styles
	// Such as color, bolds, italics, etc
	if (strcmp(rgbStr, "${R}") == 0)
	{
		rgb.__externalAction = APC_RGB_Command_Reset;

 		rgb.r = 0;
 		rgb.g = 0;
 		rgb.b = 0;

		return rgb;
	}

	else if (strcmp(rgbStr, "${BOLD}") == 0)
	{
		rgb.__externalAction = APC_RGB_Command_Bold;

 		rgb.r = 0;
 		rgb.g = 0;
 		rgb.b = 0;

		return rgb;
	}

	else if (strcmp(rgbStr, "${ITALIC}") == 0)
	{
		rgb.__externalAction = APC_RGB_Command_Italic;

 		rgb.r = 0;
 		rgb.g = 0;
 		rgb.b = 0;

		return rgb;
	}

	else if (strcmp(rgbStr, "${UNDERLINE}") == 0)
	{
		rgb.__externalAction = APC_RGB_Command_Underline;

 		rgb.r = 0;
 		rgb.g = 0;
 		rgb.b = 0;

		return rgb;
	}

	if (!rgbStr)
		return rgb;

	int r, g, b;

	// Get format: ${r,g,b}
	if (sscanf(rgbStr, "${%d,%d,%d}", &r, &g, &b) == 3)
	{
		if (r >= 0 && r <= 255) rgb.r = (unsigned char)r;
		if (g >= 0 && g <= 255) rgb.g = (unsigned char)g;
		if (b >= 0 && b <= 255) rgb.b = (unsigned char)b;
	}

	return rgb;
}

char *__apc_colorFormat(APC_ArgParser *argpar, const char *msg)
{
	CSTR result = cstr_init();
	cstr_set(&result, msg);

	if (!argpar->enableColor)
	{
		// Remove occurrences that have:
		// `${` until the next `}`
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

	else
	{
		// Replace occurrences that have:
		// `${` until the next `}`
		// With the corresponding color syntax;
		// ${r,g,b}

		int pos = 0;

		CSTR substr = cstr_init();

		while (1)
		{
			const int start = cstr_findFrom(&result, "${", pos);

			if (start == CSTR_NPOS)
				break;

			const int end = cstr_findFrom(&result, "}", start + 2);

			if (end == CSTR_NPOS)
				break;

			int len = (end - start) + 1;

			cstr_set(&substr, result.data);
			cstr_substr(&substr, start, len);

			// Set the RGB to the struct and then format it to string to
			// output it in an ANSI color format
			const APC_RGB rgb = __apc_rgbToRGBStruct(substr.data);

			char *rgbStr = NULL;

			if (rgb.__externalAction == APC_RGB_Command_Reset)
				rgbStr = APC_STYLE_RESET;

			else if (rgb.__externalAction == APC_RGB_Command_Bold)
				rgbStr = APC_STYLE_BOLD;

			else if (rgb.__externalAction == APC_RGB_Command_Italic)
				rgbStr = APC_STYLE_ITALIC;

			else if (rgb.__externalAction == APC_RGB_Command_Underline)
				rgbStr = APC_STYLE_UNDERLINE;

			else rgbStr = __apc_setRGB(rgb.r, rgb.g, rgb.b);

			// 1. Remove `${r,g,b}` with its params
			// 2. And Insert ANSI string at same position where it's supposed to be
			cstr_erase(&result, start, len);
			cstr_insert(&result, rgbStr, start);

			// Move position forward past inserted ANSI sequence
			pos = start + strlen(rgbStr);

			// If external action is made
			// DO NOT free, or else, it'll crash
			if (!rgb.__externalAction) { APC_FREE(rgbStr); }
		}

		cstr_destroy(&substr);
	}

	char *ret = CSTR_sys_strdup(result.data);

	cstr_destroy(&result);

	return ret;
}

char *apc_generateHelp(APC_ArgParser *argpar,
					   const char *title,
					   const char *topInfo,
					   const char *lowerInfo)
{
	CSTR docs = cstr_init();
	cstr_set(&docs, "");

	CSTR tmpContent = cstr_init();
	cstr_set(&tmpContent, "");

	char *result = NULL;

	// Title
	cstr_clear(&tmpContent);
	cstr_add(&tmpContent, APC_STYLECOLOR_TITLE "${BOLD}");
	cstr_add(&tmpContent, title ? title : "");
	cstr_add(&tmpContent, "${R}\n");

	result = __apc_colorFormat(argpar, tmpContent.data);

	if (result)
	{
		cstr_add(&docs, result);
		APC_FREE(result);
	}

	// Top info
	cstr_clear(&tmpContent);
	cstr_add(&tmpContent, "${ITALIC}");
	cstr_add(&tmpContent, topInfo ? topInfo : "");
	cstr_add(&tmpContent, "${R}\n");

	result = __apc_colorFormat(argpar, tmpContent.data);

	if (result)
	{
		cstr_add(&docs, result);
		APC_FREE(result);
	}

	// MAIN INFO

	for (size_t i = 0 ; i < argpar->args.size ; i++)
	{
		const APC_ArgInfo *info = cvec_get(&argpar->args, i);

		cstr_clear(&tmpContent);

		// Example:
		// help [ --help|-h|-? ]; Show this help
		cstr_add(&tmpContent, info->id);

		// Open optional
		if (!info->required)
		{
			char *parsedColor = __apc_colorFormat(argpar, APC_STYLECOLOR_OPTIONAL);

			cstr_add(&tmpContent, parsedColor);
		 	cstr_add(&tmpContent, " [ ");

		 	APC_FREE(parsedColor);
		}

		cstr_add(&tmpContent, info->param);

		if (strlen(info->sparam) > 0) 
		{
			cstr_add(&tmpContent, "|");
			cstr_add(&tmpContent, info->sparam);
		}

		for (size_t i = 0 ; i < info->aliases.size ; i++)
		{
			const char **aliasName = cvec_get(&info->aliases, i);

			if (!aliasName && !*aliasName)
				continue;

			cstr_add(&tmpContent, "|");
			cstr_add(&tmpContent, *aliasName);
		}

		// Close optional
		if (!info->required)
		{
			char *parsedColor = __apc_colorFormat(argpar, "${R}");

			cstr_add(&tmpContent, " ]");
			cstr_add(&tmpContent, parsedColor);

		 	APC_FREE(parsedColor);
		}

		cstr_add(&tmpContent, "; ");
		cstr_add(&tmpContent, info->help);
		cstr_add(&tmpContent, "\n");

		cstr_add(&docs, tmpContent.data);
	}

	//

	// Lower info
	cstr_clear(&tmpContent);
	cstr_add(&tmpContent, "${ITALIC}");
	cstr_add(&tmpContent, lowerInfo ? lowerInfo : "");
	cstr_add(&tmpContent, "${R}");

	result = __apc_colorFormat(argpar, tmpContent.data);

	if (result)
	{
		cstr_add(&docs, result);
		APC_FREE(result);
	}

	char *data = CSTR_sys_strdup(docs.data);

	cstr_destroy(&docs);
	cstr_destroy(&tmpContent);

	return data;
}

