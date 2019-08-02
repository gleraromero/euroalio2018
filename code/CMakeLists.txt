cmake_minimum_required(VERSION 2.8.4)
project(euroalio2018)

# Set compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14 -m64")
if ("${CMAKE_BUILD_TYPE}" MATCHES release OR "${CMAKE_BUILD_TYPE}" MATCHES Release)
    message("-- Compiling in Release Mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
elseif ("${CMAKE_BUILD_TYPE}" MATCHES debug OR "${CMAKE_BUILD_TYPE}" MATCHES Debug)
    message("-- Compiling in Debug Mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# Add include directory.
include_directories("include/")

# Including GOC.
add_subdirectory(goc)
include_directories(goc/include)

# Create binaries.
add_executable(euroalio2018 src/main.cpp src/vrp_instance.cpp src/preprocess_travel_times.cpp src/ttbf.cpp src/cttbf.cpp src/gcs.cpp)
target_link_libraries(euroalio2018 goc)
target_link_libraries(euroalio2018)