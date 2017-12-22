project("Raytracer")

find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")

file(GLOB raytracer_SRCS "*.cpp")
add_executable(ray ${raytracer_SRCS})
target_link_libraries(ray ${CMAKE_THREAD_LIBS_INIT})
