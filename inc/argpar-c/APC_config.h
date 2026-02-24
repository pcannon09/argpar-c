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
