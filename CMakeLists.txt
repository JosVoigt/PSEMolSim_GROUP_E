cmake_minimum_required(VERSION 3.10)

# define project name, version
project(PSEMolDyn_GroupE VERSION 0.0.1)

#define compile flags
set (FLAGS "-Wall -Werror -Wpedantic")

set (RELEASE_FLAGS "-O3 ${FLAGS}")
set (DEBUG_FLAGS "-O0 -g ${FLAGS}")
set (RELDEBUG_FLAGS "-O3 -g ${FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE_INIT "${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${RELDEBUG_FLAGS}")

option(TIDY "Enable clang tidy (off by default)" OFF)
if(TIDY) 
    set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -header-filter=src/*/*.h;
  -checks=*;
  -warnings-as-errors=*;)
endif(TIDY)

# let ccmake and cmake-gui offer the default build type options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")

# set Release as the default build type if it is not yet set.
if(NOT CMAKE_BUILD_TYPE)
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
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(doxygen)