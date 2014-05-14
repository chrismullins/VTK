cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()

project(VTKStatExample)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Add example use of vtkDescriptiveStatistics
add_executable(ExampleDescriptiveStatistics ExampleDescriptiveStatistics.cxx)
target_link_libraries(ExampleDescriptiveStatistics ${VTK_LIBRARIES})

# Add example use of vtkKMeansStatistics
add_executable(ExampleKMeansStatistics ExampleKMeansStatistics.cxx)
target_link_libraries(ExampleKMeansStatistics ${VTK_LIBRARIES})
