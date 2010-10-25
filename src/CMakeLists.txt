set(header_path ${osg_include_path})

# define the sources
set(fissure_src LoadPNG.h LoadPNG.cpp ReadPNG.cpp)

#build the program start point
add_executable(fissure ${fissure_src} )

target_link_libraries(fissure ${CMAKE_REQUIRED_LIBRARIES})

#ADD THE INSTALL TARGETS
INSTALL (TARGETS fissure RUNTIME DESTINATION bin)
		