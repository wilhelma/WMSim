include (ExternalProject)
cmake_minimum_required(VERSION 3.5)
set(THIS WMSim)

project(${THIS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-fno-omit-frame-pointer")

# sqlite--------------------------------------------------------------------------------------------
ExternalProject_Add (sqlite
        SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/sqlite
        URL http://www.sqlite.org/2018/sqlite-autoconf-3220000.tar.gz
        CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sqlite/configure
        BUILD_COMMAND ${MAKE}
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
        LOG_DOWNLOAD 1
        LOG_CONFIGURE 1
        LOG_BUILD 1)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/sqlite)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/sqlite/.libs)
# --------------------------------------------------------------------------------------------------

set(SOURCES
        src/main.cpp
        database/src/dbloader.cpp
        simulator/src/simulator.cpp
        simulator/src/wm.cpp
        visualization/src/visualizer.cpp)

set(INCLUDE_DIR include)
set(DAT_INCLUDE_DIR database/include)
set(SIM_INCLUDE_DIR simulator/include)
set(VIS_INCLUDE_DIR visualization/include)

file(GLOB_RECURSE HEADERS
        ${INCLUDE_DIR}/*.h
        ${DAT_INCLUDE_DIR}/*.h
        ${SIM_INCLUDE_DIR}/*.h
        ${VIS_INCLUDE_DIR}/*.h)

include_directories(${INCLUDE_DIR} ${DAT_INCLUDE_DIR} ${SIM_INCLUDE_DIR} ${VIS_INCLUDE_DIR})

add_executable(${THIS} ${SOURCES} ${HEADERS})
add_dependencies(${THIS} sqlite)
target_link_libraries(${THIS} sqlite3 pthread)
