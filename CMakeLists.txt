cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 17)

project(LSM6DSOX_Rpi VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

file(GLOB src "LSM*.cpp")

file(GLOB include "LSM*.h")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(LSM6DSOX_Rpi SHARED ${src})

set_target_properties(LSM6DSOX_Rpi PROPERTIES POSITION_INDEPENDENT_CODE TRUE PUBLIC_HEADER "${include}")

target_link_libraries(LSM6DSOX_Rpi i2c gpiod )

install(TARGETS LSM6DSOX_Rpi EXPORT LSM6DSOX_Rpi-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

message("library and archive destination " ${CMAKE_INSTALL_LIBDIR})

message("public header destination " ${CMAKE_INSTALL_INCLUDEDIR})
#add_subdirectory(test)

set(CMAKE_BUILD_TYPE Debug)
