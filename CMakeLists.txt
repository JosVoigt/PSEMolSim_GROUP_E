cmake_minimum_required(VERSION 3.10)

# define project name, version
project(PSEMolSim_GroupE VERSION 0.0.1)

option(DOWNLOAD_DEPENDENCIES "Downloads most dependencies (no xerces-c)" ON)
if (DOWNLOAD_DEPENDENCIES)
    include(FetchContent)

    # Declare where to fetch Google Test from
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
    set(BOOST_INCLUDE_LIBRARIES program_options)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.80.0
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
    FetchContent_MakeAvailable(Boost)
    FetchContent_MakeAvailable(fmt)
    FetchContent_MakeAvailable (xerces-c)
endif(DOWNLOAD_DEPENDENCIES)

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

include(CTest)
add_test(NAME Full_Test COMMAND MolSim -t)


# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set.
if(NOT CMAKE_BUILD_TYPE)
    message("Defaulting to release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#create build directories
file (MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/exec/bin")
set (CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/exec/bin")
set (EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/exec")

# collect all cpp files
file(GLOB_RECURSE MY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MolSim.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*/*.cpp"
    # header don't need to be included but this might be necessary for some IDEs
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h"
)

# create make target
add_executable(MolSim ${MY_SRC})

# set cxx standard. You may raise this if you want.
target_compile_features(MolSim
        PRIVATE
            cxx_std_17
)

target_include_directories(MolSim
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/libxsd
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(MolSim
        # stuff that is used in headers and source files
        PUBLIC
            xerces-c
            boost_program_options
            gtest
            gtest_main
            spdlog
            #for spdlog
            fmt
            )

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(doxygen)