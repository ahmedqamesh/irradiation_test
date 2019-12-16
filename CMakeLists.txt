#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Xray_Test)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Xray_Test Xray_Test.cc ${sources} ${headers})
target_link_libraries(Xray_Test ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Xray_Test. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
# Xray_Test.in Xray_Test.out
set(Xray_Test_SCRIPTS
    Test01.mac gammaSpectrum_Tungsten-Spectrum.dat gammaSpectrum_Be-0.3mm-Spectrum.dat gammaSpectrum_Al-0.15mm-Spectrum.dat File1.dat  gammaSpectrum_RD53.mac gammaSpectrum_RD54_No.mac icons.mac beam.mac gammaSpectrum_Al-0.15mm-Spectrum.mac gammaSpectrum_Tungsten-Spectrum.mac gammaSpectrum_Be-0.3mm-Spectrum.mac retrieveTables.mac storeTables.mac  run.png init_vis_gui.mac gui.mac vis.mac
  )

foreach(_script ${Xray_Test_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Xray_Test DESTINATION bin)

