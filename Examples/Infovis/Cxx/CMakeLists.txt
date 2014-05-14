cmake_minimum_required(VERSION 2.8)
project(Infovis)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(TreeLayout TreeLayout.cxx)
target_link_libraries(TreeLayout ${VTK_LIBRARIES})

add_executable(HelloWorld HelloWorld.cxx)
target_link_libraries(HelloWorld ${VTK_LIBRARIES})

add_executable(Theme Theme.cxx)
target_link_libraries(Theme ${VTK_LIBRARIES})

add_executable(CreateTree CreateTree.cxx)
target_link_libraries(CreateTree ${VTK_LIBRARIES})

add_executable(MultiView MultiView.cxx)
target_link_libraries(MultiView ${VTK_LIBRARIES})

add_subdirectory(EasyView)
add_subdirectory(CustomLinkView)
add_subdirectory(StatsView)


IF(VTK_USE_PARALLEL_BGL)
  ADD_EXECUTABLE(ParallelBFS ParallelBFS.cxx)
  find_package(MPI REQUIRED)
  TARGET_LINK_LIBRARIES(ParallelBFS vtkParallel vtkViews ${MPI_C_LIBRARIES})
  if (MPI_CXX_LIBRARIES)
    TARGET_LINK_LIBRARIES(ParallelBFS ${MPI_CXX_LIBRARIES})
  endif()
ENDIF(VTK_USE_PARALLEL_BGL)
