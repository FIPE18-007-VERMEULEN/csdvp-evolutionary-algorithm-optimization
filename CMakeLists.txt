######################################################################################
### 0) Set your application properties
######################################################################################
# check cmake version compatibility
cmake_minimum_required(VERSION 2.6)

# Here define your project name
project(CEAO)

# Here define the name and the version of your package
set(PACKAGE_NAME "CEAO" CACHE STRING "Package name" FORCE)
set(PACKAGE_VERSION "1.0" CACHE STRING "aPackage version" FORCE)


# regular expression checking
include_regular_expression("^.*$" "^$")

# set a language for the entire project.
enable_language(CXX)
enable_language(C)

######################################################################################

######################################################################################

######################################################################################
### 2) Include the sources
######################################################################################
######################################
### Include required modules & utilities
#####################################################################################
include(CMakeBackwardCompatibilityCXX)

include(FindDoxygen)

include(CheckLibraryExists)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/module")
find_package(Paradiseo COMPONENTS eoutils eo)

include_directories(${PARADISEO_INCLUDE_DIR})
              
######################################################################################

#####################################################################################
### Manage the build type
#####################################################################################

# the user should choose the build type on windows environments,excepted under cygwin (default=none)
set(CMAKE_DEFAULT_BUILD_TYPE "Release" CACHE STRING "Variable that stores the default CMake build type" FORCE)

find_program(MEMORYCHECK_COMMAND
    NAMES purify valgrind
    PATHS
    "/home/alexis/.local/share/paradiseo /usr/local/bin /usr/bin [HKEY_LOCAL_MACHINE\\SOFTWARE\\Rational Software\\Purify\\Setup;InstallFolder]"
    DOC "Path to the memory checking command, used for memory error detection.") 
 
      
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DCMAKE_VERBOSE_MAKEFILE=ON)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions(-DDEPRECATED_MESSAGES)

######################################################################################
### 3) Link the librairies for your executable
######################################################################################

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(application)

######################################################################################

