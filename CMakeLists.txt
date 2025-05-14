cmake_minimum_required(VERSION 3.10)
project(skim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)

# Find GTKmm
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
message(STATUS "Found GTKmm version ${GTKMM_VERSION}")

# Find WebKitGTK - you have the 6.0 version
pkg_check_modules(WEBKIT REQUIRED webkitgtk-6.0)
message(STATUS "Found WebKit version ${WEBKIT_VERSION}")

# Find Lua
pkg_check_modules(LUA REQUIRED lua5.3)
if(NOT LUA_FOUND)
    pkg_check_modules(LUA QUIET lua5.4)
    if(NOT LUA_FOUND)
        pkg_check_modules(LUA QUIET lua)
    endif()
endif()

# Include directories and libraries
include_directories(${GTKMM_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS} ${WEBKIT_LIBRARY_DIRS} ${LUA_LIBRARY_DIRS})

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(skim ${SOURCES})

# Link libraries
target_link_libraries(skim
    ${GTKMM_LIBRARIES}
    ${WEBKIT_LIBRARIES}
    ${LUA_LIBRARIES}
)

# Install target
install(TARGETS skim DESTINATION bin)
