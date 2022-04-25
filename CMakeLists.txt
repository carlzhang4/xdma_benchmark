cmake_minimum_required (VERSION 3.1)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/")

SET(CMAKE_C_COMPILER "/usr/bin/gcc-7")   # must be above project
SET(CMAKE_CXX_COMPILER "/usr/bin/g++-7")


project (dma_test LANGUAGES CXX CUDA)

find_package(CUDA)

SET(CUDA_SEPARABLE_COMPILATION ON)#do not add this line above the find package
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set (CMAKE_CXX_STANDARD 14)

add_definitions(-mavx512f -mavx512vl)
add_definitions("-Wall -g")
set(CUDA_NVCC_FLAGS --default-stream per-thread)

CUDA_ADD_EXECUTABLE(dma_test
	dma_test.cpp
	benchmark.cu
    )

target_link_libraries(dma_test
	cuda
	cudart
	pthread
	gdrapi
	libxdma.a
    )
#${TORCH_LIBRARIES}
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

