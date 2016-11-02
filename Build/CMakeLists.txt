cmake_minimum_required(VERSION 2.8)
project( LaneDetection  )

# Locate libraries and headers
find_package( OpenCV REQUIRED )
find_package( Threads )

# Include files
file(GLOB SOURCES
#	../src/Controller/testGoingStraight.cpp
#	../src/CarCtrl.cpp
	../src/*.cpp
)
# Link against libraries to executable file
add_executable( LaneDetection  ${SOURCES})

target_link_libraries(LaneDetection ${OpenCV_LIBS} )
target_link_libraries (LaneDetection ${CMAKE_THREAD_LIBS_INIT})
