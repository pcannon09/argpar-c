# PCANNON CMAKELISTS.TXT v1.3 - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20260216
# https://github.com/pcannon09/pcannonProjectStandards

cmake_minimum_required(VERSION 3.24)

include(CMakePackageConfigHelpers)

# Read project metadata

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.private/project.json" projectInfo)

string(JSON projectName		GET "${projectInfo}" exeName)
string(JSON projectVersion	 GET "${projectInfo}" version)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD  GET "${projectInfo}" versionSTD)

project(${projectName}
	VERSION ${projectVersion}
	LANGUAGES C
)

# C Standard

set(C_STD 11)
set(CMAKE_C_STANDARD ${C_STD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Directories

set(SRC_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
set(INC_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/inc/${projectName}")

# Sources

set(HEADERS
	${INC_DIR}/APCpredefines.h
	${INC_DIR}/APC_config.h

	${INC_DIR}/lib/APC_ArgParser.h
)

set(SOURCES
	${SRC_DIR}/lib/APC_ArgParser.c
)

# Library Targets

add_library(${projectName} STATIC ${SOURCES} ${HEADERS})
add_library(${projectName}::${projectName} ALIAS ${projectName})

target_include_directories(${projectName}
	PUBLIC
		$<BUILD_INTERFACE:${INC_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${projectName}>
)

if(NOT TARGET ${projectName}_uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(${projectName}_uninstall
        COMMAND ${CMAKE_COMMAND} -P
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        COMMENT "Uninstalling ${PROJECT_NAME}..."
    )
endif()

target_compile_features(${projectName} PUBLIC c_std_${C_STD})

# Compiler Warnings

if(MSVC)
	target_compile_options(${projectName} PRIVATE /W4)
else()
	target_compile_options(${projectName} PRIVATE
		-Wall -Wextra -Wpedantic
	)
endif()

# Debug / Release handling

target_compile_definitions(${projectName}
	PUBLIC
		$<$<CONFIG:Debug>:APC_DEV=1>
		$<$<CONFIG:Release>:APC_DEV=0>
)

add_executable(${projectName}_exec ${SRC_DIR}/main.c)
target_link_libraries(${projectName}_exec PRIVATE
		${projectName}

		cvec_static
		cstr_static
)

if(NOT MSVC)
	target_compile_options(${projectName} PRIVATE
		$<$<CONFIG:Debug>:-g>
		$<$<CONFIG:Release>:-O3>
	)

	target_link_options(${projectName} PRIVATE
		$<$<CONFIG:Debug>:-g>
	)

	target_compile_options(${projectName}_exec PRIVATE
		$<$<CONFIG:Debug>:-g>
		$<$<CONFIG:Release>:-O3>
	)

	target_link_options(${projectName}_exec PRIVATE
		$<$<CONFIG:Debug>:-g>
	)
endif()

# Optional sanitizers (only for Debug and only on supported compilers)
option(ENABLE_SANITIZER "Enable address sanitizer in Debug builds" ON)

if(ENABLE_SANITIZER AND NOT MSVC)
	target_compile_options(${projectName} PRIVATE
		$<$<CONFIG:Debug>:-fsanitize=address,leak>
	)

	target_link_options(${projectName} PRIVATE
		$<$<CONFIG:Debug>:-fsanitize=address,leak>
	)

	target_compile_options(${projectName}_exec PRIVATE
		$<$<CONFIG:Debug>:-fsanitize=address,leak>
	)

	target_link_options(${projectName}_exec PRIVATE
		$<$<CONFIG:Debug>:-fsanitize=address,leak>
	)
endif()

# Executables

add_subdirectory(${VENDOR_DIR}/cvec cvec)
add_subdirectory(${VENDOR_DIR}/cstr cstr)

set_target_properties(
	${projectName}
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install Rules

install(TARGETS ${projectName}
	EXPORT ${projectName}Targets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${INC_DIR}/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${projectName}
)

# Package Config

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_package_config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${projectName}
)

install(EXPORT ${projectName}Targets
	FILE ${projectName}Targets.cmake
	NAMESPACE ${projectName}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${projectName}
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${projectName}
)

# CPack

set(CPACK_PACKAGE_NAME ${projectName})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "pcannon")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "pcannon project standards TEMPLATE")

include(CPack)

# Build Info

message(STATUS "Project:		${projectName}")
message(STATUS "Version:		${PROJECT_VERSION}-${projectVersionState}")
message(STATUS "C Standard:		C${C_STD}")
message(STATUS "Compiler:		${CMAKE_C_COMPILER}")

