cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()

PROJECT (Modelling)

find_package(VTK COMPONENTS
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersGeneral
  vtkIOXML
  vtkImagingHybrid
  vtkInteractionStyle
  vtkRenderingOpenGL
  vtkTestingRendering
)
include(${VTK_USE_FILE})

add_executable(finance MACOSX_BUNDLE finance.cxx)
target_link_libraries(finance ${VTK_LIBRARIES})

add_executable(Delaunay3D MACOSX_BUNDLE Delaunay3D.cxx)
target_link_libraries(Delaunay3D ${VTK_LIBRARIES})

add_executable(Delaunay3DAlpha MACOSX_BUNDLE Delaunay3DAlpha.cxx)
target_link_libraries(Delaunay3DAlpha ${VTK_LIBRARIES})

if(BUILD_TESTING)
  enable_testing()
  if(vtkTestingRendering_LOADED)
    if("${VTK_TEST_DATA_DIR}" STREQUAL "")
      message(FATAL_ERROR "Please provide a test data directory.")
    endif()
    ######## Regression Testing ########
    set(TestFinance_ARGS "${VTK_TEST_DATA_DIR}/Data/financial.txt")
    set(TestDelaunay3D_ARGS
      "${VTK_TEST_DATA_DIR}/Data/cow.vtp"
      ${CMAKE_BINARY_DIR}/Delaunay3D.vtu
      )
    set(TestDelaunay3DAlpha_ARGS
      .5 "${VTK_TEST_DATA_DIR}/Data/cow.vtp"
      ${CMAKE_BINARY_DIR}/Delaunay3DAlpha.vtu
      )
    add_test(TestFinance finance ${TestFinance_ARGS})
    add_test(TestDelaunay3D Delaunay3D ${TestDelaunay3D_ARGS})
    add_test(TestDelaunay3DAlpha Delaunay3DAlpha ${TestDelaunay3DAlpha_ARGS})
  endif()
endif()
