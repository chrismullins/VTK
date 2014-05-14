cmake_minimum_required(VERSION 2.8)
project(Charts)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(GraphItem GraphItem.cxx vtkGraphItem.cxx)
target_link_libraries(GraphItem ${VTK_LIBRARIES})

add_executable(PiecewiseItem PiecewiseItem.cxx)
target_link_libraries(PiecewiseItem ${VTK_LIBRARIES})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
add_executable(QChartTable QChartTable.cxx)
target_link_libraries(QChartTable
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES} )

add_executable(QScalarsToColors QScalarsToColors.cxx)
target_link_libraries(QScalarsToColors
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES} )
