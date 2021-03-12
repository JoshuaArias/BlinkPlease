cmake_minimum_required( VERSION 3.19.5 )
project( program )

# include dlib
include( FetchContent )
FetchContent_Declare( dlib
    GIT_REPOSITORY https://github.com/davisking/dlib.git
    GIT_TAG        v19.18
    )
FetchContent_MakeAvailable( dlib )

# include opencv
# sudo apt-get install libopencv-dev
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# add includes
include_directories( "include" )

# add executable
add_executable( program
    src/main.cpp
    src/App.cpp
    src/Blink.cpp
    src/Eye.cpp
    src/Monitor.cpp
    src/Rest.cpp
    include/App.hpp
    include/Blink.hpp
    include/Eye.hpp
    include/Monitor.hpp
    include/Rest.hpp
)

# link libraries
target_link_libraries( program dlib::dlib ${OpenCV_LIBS} )