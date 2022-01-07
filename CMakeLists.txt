cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
option(PLATFORM_LINUX "Linux platform target" OFF)
option(PLATFORM_WINDOWS "Windows platform target (msys/mingw64)" OFF)
option(PLATFORM_SWITCH "Nintendo Switch platform target" OFF)
option(PLATFORM_3DS "Nintendo 3DS platform target" OFF)
option(PLATFORM_VITA "Sony PS Vita platform target" OFF)
option(PLATFORM_PS4 "Sony PS Vita platform target" OFF)

include(Toolchain.cmake)

project(pscrap)

##############
# PSCRAP
##############
set(CMAKE_MODULE_PATH cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(CURL REQUIRED)
find_package(JSON-C REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB SOURCES source/*.c*)
set(INCLUDES include ${CURL_INCLUDE_DIRS} ${JSON-C_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(LDFLAGS ${CURL_LIBRARIES} ${JSON-C_LIBRARIES} ${ZLIB_LIBRARIES})

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND CFLAGS -D__SWITCH__)
    list(APPEND INCLUDES)
    list(APPEND LDFLAGS mbedtls mbedcrypto mbedx509 nx)
elseif (PLATFORM_PS4)
    list(APPEND LDFLAGS json-c mbedtls mbedcrypto mbedx509 SceNet)
elseif (PLATFORM_LINUX)
    #####################
    # LINUX PLATORM
    #####################
    list(APPEND CFLAGS)
    list(APPEND LDFLAGS)
endif ()

#####################
# PSCRAP LIBRARY
#####################
# read tmdb api key from file, required
file(STRINGS "tmdb.key" TMDB_API_KEY)
add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
target_compile_options(${PROJECT_NAME} PUBLIC -DAPI_KEY=\"${TMDB_API_KEY}\")
target_compile_options(${PROJECT_NAME} PRIVATE ${CFLAGS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LDFLAGS})

#####################
# PSCRAP TEST
#####################
add_executable(${PROJECT_NAME}.elf test/main.cpp)
target_compile_options(${PROJECT_NAME}.elf PUBLIC -DAPI_KEY=\"${TMDB_API_KEY}\")
target_link_libraries(${PROJECT_NAME}.elf ${PROJECT_NAME})
