cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(kea_capture LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(tof REQUIRED)
find_package(OpenMP REQUIRED)

set(kea_capture_srcs src/capture.cpp)

add_executable(kea_capture ${kea_capture_srcs})
add_executable(kea_reader src/reader.cpp)

target_link_libraries(kea_capture PRIVATE OpenMP::OpenMP_CXX tof::tof)
target_link_libraries(kea_reader PRIVATE OpenMP::OpenMP_CXX tof::tof)

install(TARGETS kea_capture DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS kea_reader DESTINATION ${CMAKE_INSTALL_BINDIR})
