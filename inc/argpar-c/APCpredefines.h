/**
 * APCpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 */

#ifndef INCLUDE_C_APCPREDEFINES_H_
#define INCLUDE_C_APCPREDEFINES_H_

// Project setup
#define APC_DEFAULT_C_STD			201112L

// Versioning
#define APC_VERSION_MAJOR            1
#define APC_VERSION_MINOR            0
#define APC_VERSION_PATCH            0

#define APC_VERSION_STD              20260226L

// Version states:
// * dev
// * beta
// * build
#define APC_VERSION_STATE          "dev"

#define APC_VERSION                ((EXAMPLE_VERSION_MAJOR<<16)|(EXAMPLE_VERSION_MINOR<<8)|(EXAMPLE_VERSION_PATCH)|(EXAMPLE_VERSION_STATE << 24))

#define APC_VERSION_CHECK(EXAMPLE_VERSION_MAJOR, EXAMPLE_VERSION_MINOR, EXAMPLE_VERSION_PATCH, EXAMPLE_VERSION_STATE) \
    (((APC_VERSION_MAJOR)<<16)|((EXAMPLE_VERSION_MINOR)<<8)|(EXAMPLE_VERSION_PATCH)|((EXAMPLE_VERSION_STATE) << 24))

// Macro utils
#define APC_STRINGIFY(x) #x
#define APC_TOSTRING(x) EXAMPLE_STRINGIFY(x)

#ifndef APC_DEV
#   define APC_DEV true
#endif

#define APC_FREE(x) free(x); x = NULL

#if defined(WIN32) || defined(_WIN32)
#	define APC_OS_WIN32
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define APC_OS_MACOS
#elif defined(__linux__)
#	define APC_OS_UNIX_LINUX
#elif defined(__unix) || defined(__unix__)
#	define APC_OS_UNIX
#elif defined(__FreeBSD__)
#	define APC_OS_FREEBSD
#else
#	error "Current platform might not supported"
#endif // defined(WIN32) // Platform check

#endif  // INCLUDE_C_APCPREDEFINES_H_

