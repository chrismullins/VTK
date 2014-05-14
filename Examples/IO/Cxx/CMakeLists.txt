cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()

project(IO)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(DumpXMLFile MACOSX_BUNDLE DumpXMLFile.cxx)
target_link_libraries(DumpXMLFile ${VTK_LIBRARIES})

add_executable(ParticleReader MACOSX_BUNDLE ParticleReader.cxx)
target_link_libraries(ParticleReader ${VTK_LIBRARIES})

if(BUILD_TESTING)
  enable_testing()
  if(vtkTestingRendering_LOADED)
    if("${VTK_TEST_DATA_DIR}" STREQUAL "")
      message(FATAL_ERROR "Please provide a test data directory.")
    endif()
    ######## Regression Testing ########
    set(TestDumpXMLFile_ARGS "${VTK_TEST_DATA_DIR}/cow.vtp")
    set(TestParticleReader_ARGS
      "${VTK_TEST_DATA_DIR}/Data/golf.csv"
       ${CMAKE_BINARY_DIR}/TestParticleReader.vtp)
    add_test(DumpXMLFileCxxTest DumpXMLFile ${TestDumpXMLFile_ARGS})
    add_test(ParticleReaderCxxTest ParticleReader ${TestParticleReader_ARGS})
  endif()
endif()
