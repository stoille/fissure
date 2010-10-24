cmake_minimum_required (2.6)
project (fissure)
#The version number.
set (fissure_VERSION_MAJOR 0)
set (fissure_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${fissure_SOURCE_DIR}/src/fissureConfig.h.in"
  "${fissure_BINARY_DIR}/src/fissureConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${fissure_BINARY_DIR}/src","${OSG_LIB}")

# add the executable
add_executable(fissure fissure.cpp)




