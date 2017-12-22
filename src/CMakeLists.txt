project("Pathtracer")

find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

file(GLOB pathtracer_SRCS "*.cpp")
add_executable(pt ${pathtracer_SRCS})
target_link_libraries(pt ${CMAKE_THREAD_LIBS_INIT})
