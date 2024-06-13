cmake_minimum_required(VERSION 3.10)

# define project name, version
project(PSEMolSim_GroupE VERSION 0.0.1)
#for clangd
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DOWNLOAD_DEPENDENCIES "Downloads dependencies" ON)
if (DOWNLOAD_DEPENDENCIES)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0  # adjust this to the version you want
    )
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
    )
    FetchContent_Declare (
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
    )
    FetchContent_Declare (
        xerces-c
        GIT_REPOSITORY https://github.com/apache/xerces-c.git
        GIT_TAG v3.2.5
    )
    FetchContent_MakeAvailable(googletest)
    FetchContent_MakeAvailable(spdlog)
    FetchContent_MakeAvailable(fmt)
    FetchContent_MakeAvailable(xerces-c)
endif(DOWNLOAD_DEPENDENCIES)

option(DOWNLOAD_BOOST "Downloads Boost, not recommended" OFF)
if (DOWNLOAD_BOOST)
    #only compile boost:program_options
    set(BOOST_INCLUDE_LIBRARIES program_options)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.80.0
    )
    FetchContent_MakeAvailable(Boost)
endif (DOWNLOAD_BOOST)
	

set (CMAKE_CXX_STANDARD 17)

#define compile flags
set (FLAGS "-Wall -Werror -Wpedantic")

set (RELEASE_FLAGS "-O3 ${FLAGS}")
set (DEBUG_FLAGS "-O0 -g ${FLAGS}")
set (RELDEBUG_FLAGS "-O3 -g ${FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE_INIT "${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${RELDEBUG_FLAGS}")

option(OUTPUT "Enable file output (on by default)" ON) 
if(NOT OUTPUT)
    add_compile_definitions(NO_OUT_FILE)
endif(NOT OUTPUT)

option(LOGGING "Enable non-critical logging (on by default)" ON)
if (NOT LOGGING)
	add_compile_definitions(NO_LOG)
endif(NOT LOGGING)

#----------------------------TESTING-----------------------#
include(CTest)
add_test(NAME Full_Test COMMAND MolSimTest)

#----------------------------BUILD TYPE--------------------#
# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set.
if(NOT CMAKE_BUILD_TYPE)
    message("Defaulting to release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#-----------------------------RESULTING DIRECTORY-----------#
#create build directories
file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/exec/bin")
set (CMAKE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/exec/bin")
set (EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}")

#-----------------------------COLLECT SRC FILES--------------#
file(GLOB_RECURSE molsimfiles
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h"
)

#-----------------------------COLLECT TEST FILES-------------#
file (GLOB_RECURSE molsimtests
${CMAKE_CURRENT_SOURCE_DIR}/test/*/*.cpp
)

#-----------------------------BUILD PROJECT LIB--------------#
add_library ( ${PROJECT_NAME} STATIC
${molsimfiles}
)

#-----------------------------LINK PROJECT LIB---------------#
target_link_libraries (${PROJECT_NAME}
PUBLIC
    xerces-c
    boost_program_options
    spdlog
    fmt #spdlog dependency
    expat
)

#-----------------------------INCLUDE DIRECTORIES------------#
target_include_directories (${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#-----------------------------TEST EXECUTABLE----------------#
add_executable (MolSimTest ${CMAKE_CURRENT_SOURCE_DIR}/test/test_suite_all.cpp ${molsimtests})
target_link_libraries (MolSimTest
    ${PROJECT_NAME}
    #linking gtest only for testing not the actual exec.
    gtest
    gtest_main
)
target_include_directories (MolSimTest
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#-----------------------------SRC EXECUTABLE-----------------#
add_executable (MolSim ${CMAKE_CURRENT_SOURCE_DIR}/src/MolSim.cpp)
target_link_libraries (MolSim
    ${PROJECT_NAME}
)
target_include_directories (MolSim
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

