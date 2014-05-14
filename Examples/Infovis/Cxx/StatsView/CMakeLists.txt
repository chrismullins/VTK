cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
project(StatsView)


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
# use what QVTK built with
set(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
set(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
set(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")



# Use the include path and library for Qt that is used by VTK.
include_directories(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)


# Set your files and resources here
set(StatsViewSrcs main.cxx StatsView.cxx)
set(StatsViewUI StatsView.ui)
set(StatsViewHeaders StatsView.h)
set(StatsViewResources Icons/icons.qrc)

# The rest should just work (sure...)
qt4_wrap_ui(UISrcs ${StatsViewUI})
qt4_wrap_cpp(MOCSrcs ${StatsViewHeaders} )
qt4_add_resources(ResourceSrcs ${StatsViewResources})

source_group("Resources" FILES
  ${StatsViewUI}
  ${StatsViewResources}
  ${EXE_ICON}
)

source_group("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
SET_SOURCE_FILES_PROPERTIES(${StatsViewSrcs} PROPERTIES
                            OBJECT_DEPENDS "${UISrcs}")

# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  SET (StatsViewSrcs ${StatsViewSrcs} ${StatsViewUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  SET (StatsViewSrcs ${StatsViewSrcs} ${StatsViewUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")

add_executable( StatsView MACOSX_BUNDLE
  ${StatsViewSrcs}
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs})
target_link_libraries( StatsView ${QT_LIBRARIES} ${VTK_LIBRARIES})
