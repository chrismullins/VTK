cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
project(EasyView)


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")


# Use the include path and library for Qt that is used by VTK.
include_directories(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)




# Set your files and resources here
set(EasyViewSrcs main.cxx EasyView.cxx)
set(EasyViewUI EasyView.ui)
set(EasyViewHeaders EasyView.h)
set(EasyViewResources Icons/icons.qrc)

# The rest should just work (sure...)
qt4_wrap_ui(UISrcs ${EasyViewUI})
qt4_wrap_cpp(MOCSrcs ${EasyViewHeaders} )
qt4_add_resources(ResourceSrcs ${EasyViewResources})

source_group("Resources" FILES
  ${EasyViewUI}
  ${EasyViewResources}
  ${EXE_ICON}
)

source_group("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

add_definitions(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
SET_SOURCE_FILES_PROPERTIES(${EasyViewSrcs} PROPERTIES
                            OBJECT_DEPENDS "${UISrcs}")

# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  SET (EasyViewSrcs ${EasyViewSrcs} ${EasyViewUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  SET (EasyViewSrcs ${EasyViewSrcs} ${EasyViewUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")

ADD_EXECUTABLE( EasyView MACOSX_BUNDLE
  ${EasyViewSrcs}
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs})
TARGET_LINK_LIBRARIES(EasyView ${VTK_LIBRARIES})



