cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing this options
option(BUILD_LINUX "Build with SDL2 support" OFF)
option(BUILD_WINDOWS "Build with Windows support (msys/mingw64)" OFF)
option(BUILD_VITA "Build with Ps Vita support (hardware rendering, fast)" OFF)
option(BUILD_PS3 "Build with PS3 support (hardware rendering, fast)" OFF)
option(BUILD_3DS "Build with 3DS support" OFF)
option(BUILD_SWITCH "Build with Nintendo Switch support" OFF)

include(Toolchain.cmake)

project(pscrap)

##############
# PSCRAP
##############
set(INCLUDES
        include
        )

file(GLOB SOURCES
        source/*.c*
        )

set(CFLAGS
        )

set(LDFLAGS curl json-c z)

#####################
# PLATORM SPECIFIC
#####################
if (BUILD_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND CFLAGS -D__SWITCH__)
    list(APPEND INCLUDES)
    list(APPEND LDFLAGS mbedtls mbedcrypto mbedx509 nx)
elseif (BUILD_LINUX)
    #####################
    # LINUX PLATORM
    #####################
    list(APPEND CFLAGS)
    list(APPEND LDFLAGS)
endif ()

#####################
# PSCRAP LIBRARY
#####################
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CFLAGS})
target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS})

#####################
# PSCRAP TEST
#####################
add_executable(${PROJECT_NAME}.elf test/main.cpp)
target_link_libraries(${PROJECT_NAME}.elf ${PROJECT_NAME})