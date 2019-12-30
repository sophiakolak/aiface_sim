cmake_minimum_required(VERSION 3.10)
set(CMAKE_CUDA_COMPILER /usr/local/cuda-10.1/bin/nvcc)
project(myproject LANGUAGES CXX CUDA)
#set(CMAKE_PREFIX_PATH "/home/sophia/Titan/cmake")
set(CMAKE_PREFIX_PATH "/home/sophia/vcpkg/installed/x64-linux/share/titan")

set(CMAKE_CXX_STANDARD 17)

add_executable(face_sim face_main.cpp)
find_package(Titan CONFIG REQUIRED)
target_link_libraries(face_sim PRIVATE Titan)
