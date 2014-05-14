cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(MPI REQUIRED)
include_directories(${MPI_USE_FILE})

add_executable(ParallelIso ParallelIso.cxx)
target_link_libraries(ParallelIso
  ${VTK_LIBRARIES}
  ${MPI_CXX_LIBRARIES}
  ${MPI_C_LIBRARIES} )

set(TaskParallelism_SRCS
  TaskParallelism.cxx
  TaskParallelism.h
  task1.cxx
  task2.cxx )

add_executable(TaskParallelism ${TaskParallelism_SRCS})
target_link_libraries(TaskParallelism
  ${VTK_LIBRARIES}
  ${MPI_CXX_LIBRARIES}
  ${MPI_C_LIBRARIES} )

add_executable(PipelineParallelism PipelineParallelism.cxx)
target_link_libraries(PipelineParallelism
  ${VTK_LIBRARIES}
  ${MPI_CXX_LIBRARIES}
  ${MPI_C_LIBRARIES} )
