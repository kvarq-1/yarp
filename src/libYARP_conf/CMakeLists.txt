# Copyright: (C) 2009, 2010 RobotCub Consortium
#                2016, 2017 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Paul Fitzpatrick, Giorgio Metta, Lorenzo Natale, Daniele E. Domenichelli
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


project(YARP_conf)

#########################################################################
# Generate compiler.h header

# Read the file containing all the documentation for compiler.h
# Change offset here in case include/yarp/conf/compiler.dox.in comment changes
file(READ
     "${CMAKE_CURRENT_SOURCE_DIR}/include/yarp/conf/compiler.dox.in"
     _compiler_dox
     OFFSET 97)


# WriteCompilerDetectionHeader is supported since CMake 3.1, but until 3.3 there
# is a bug in the non-c++11 compatible nullptr implementation, therefore we use
# a pre-generated version of the file
if(NOT ${CMAKE_MINIMUM_REQUIRED_VERSION} VERSION_LESS 3.3)
  message(AUTHOR_WARNING "CMAKE_MINIMUM_REQUIRED_VERSION is now ${CMAKE_MINIMUM_REQUIRED_VERSION}. This check can be removed.")
endif()
if(${CMAKE_VERSION} VERSION_LESS 3.3)
  string(REPLACE "\\\;" ";" _compiler_dox ${_compiler_dox})
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/yarp/conf/compiler.h.in"
                 "${PROJECT_BINARY_DIR}/include/yarp/conf/compiler.h"
                 @ONLY)
else()
  include(WriteCompilerDetectionHeader)

  get_property(_cxx_known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)

  write_compiler_detection_header(
    FILE "${PROJECT_BINARY_DIR}/include/yarp/conf/compiler.h"
    PREFIX YARP
    COMPILERS
      GNU
      Clang
      AppleClang
      MSVC
    FEATURES ${_cxx_known_features}
    VERSION 3.1.0
    PROLOG ${_compiler_dox})
endif()



#########################################################################
# Generate the other headers

foreach(conf version system options api numeric)
  set(outconf "${PROJECT_BINARY_DIR}/include/yarp/conf/${conf}.h")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/yarp/conf/${conf}.h.in"
                 "${outconf}")
endforeach(conf)


#########################################################################
# Create the YARP_conf interface library

add_library(YARP_conf INTERFACE)
add_library(YARP::YARP_conf ALIAS YARP_conf)

target_include_directories(YARP_conf INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                                               $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


#########################################################################
# Install all the headers

install(TARGETS YARP_conf
        EXPORT YARP
        COMPONENT YARP_conf)

install(DIRECTORY "${PROJECT_BINARY_DIR}/include/yarp/conf"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/yarp
        FILES_MATCHING PATTERN "*.h")

set_property(GLOBAL APPEND PROPERTY YARP_LIBS YARP_conf)


#########################################################################
# Set YARP_TREE_INCLUDE_DIRS global property

set_property(GLOBAL APPEND PROPERTY YARP_TREE_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/include")
