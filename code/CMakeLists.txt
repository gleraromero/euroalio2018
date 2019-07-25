cmake_minimum_required(VERSION 2.8.4)
project(euroalio2018)

include_directories("include/")
message($ENV{CPLEX_INCLUDE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14 -m64")
if ("${CMAKE_BUILD_TYPE}" MATCHES release OR "${CMAKE_BUILD_TYPE}" MATCHES Release)
    message("-- Compiling in Release Mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
elseif ("${CMAKE_BUILD_TYPE}" MATCHES debug OR "${CMAKE_BUILD_TYPE}" MATCHES Debug)
    message("-- Compiling in Debug Mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# Check if CPLEX environment variables are set.
if("$ENV{CPLEX_INCLUDE}" STREQUAL "")
    message(SEND_ERROR "CPLEX_INCLUDE environment variable is not set. Make sure to point the variable to the CPLEX include directory before compiling.\nTo do this run on the terminal (for example):\nexport CPLEX_INCLUDE=/opt/ibm/ILOG/CPLEX_Studio129/cplex/include\nThis can be added permanently by adding the export command to the file /etc/environment and rebooting (on Linux).")
endif()
if("$ENV{CPLEX_BIN}" STREQUAL "")
    message(SEND_ERROR "CPLEX_BIN environment variable is not set. Make sure to point the variable to the CPLEX binary (.a) before compiling.\nTo do this run on the terminal (for example):\nexport CPLEX_BIN=/opt/ibm/ILOG/CPLEX_Studio129/cplex/lib/x86-64_linux/static_pic/libcplex.a\nThis can be added permanently by adding the export command to the file /etc/environment and rebooting (on Linux).")
endif()

# Including GOC.
add_subdirectory(goc)
include_directories(goc/include)

# Create binaries.
include_directories($ENV{CPLEX_INCLUDE})
add_executable(euroalio2018 src/main.cpp src/vrp_instance.cpp src/preprocess_travel_times.cpp src/ttbf.cpp src/cttbf.cpp src/gcs.cpp)
target_link_libraries(euroalio2018 goc)
target_link_libraries(euroalio2018 $ENV{CPLEX_BIN} -ldl -lm)