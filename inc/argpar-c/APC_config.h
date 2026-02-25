#ifndef INCLUDE_ARGPAR_C_APC_CONFIG_H_
#define INCLUDE_ARGPAR_C_APC_CONFIG_H_

#ifdef __cplusplus
# 	define __APC_CPP_GUARD_OPEN 		extern "C" {
# 	define __APC_CPP_GUARD_CLOSE 		}
#else
# 	define __APC_CPP_GUARD_OPEN
# 	define __APC_CPP_GUARD_CLOSE
#endif // __cplusplus 

__APC_CPP_GUARD_OPEN

#define APC_STYLE_RESET 				"\033[0m"
#define APC_STYLE_BOLD 					"\033[1m"
#define APC_STYLE_ITALIC				"\033[3m"
#define APC_STYLE_UNDERLINE 			"\033[4m"
#define APC_STYLE_STRIKETRHOUGH 		"\033[9m"

#define APC_STYLECOLOR_TITLE 			"${245,245,245}" // rgb(245,245,245)
#define APC_STYLECOLOR_OPTIONAL			"${150,150,150}" // rgb(150,150,150)
#define APC_STYLECOLOR_IMPORTANT		"${255,71,76}" // rgb(255,71,76)
#define APC_STYLECOLOR_WARNING			"${255,255,40}" // rgb(255,255,40)

#define APC_Declare_Pair(_first, _second, _name) \
	typedef struct APC_Pair##_name \
	{ \
		_first first; \
		_second second; \
	} APC_Pair##_name; \

__APC_CPP_GUARD_CLOSE

#undef __APC_CPP_GUARD_OPEN
#undef __APC_CPP_GUARD_CLOSE

#endif  // INCLUDE_ARGPAR-C_APC_CONFIG_H_
