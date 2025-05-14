cmake_minimum_required(VERSION 3.10)
project(skim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)  # Changed from 4.0 to 4.1
pkg_check_modules(LUA REQUIRED lua5.3)

# Include directories
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
