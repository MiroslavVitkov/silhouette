project( silhouette )

cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options( -Wall -Wextra -pedantic -Werror -Wfatal-errors ${OpenMP_CXX_FLAGS} )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules" )
find_package( dlib REQUIRED )
find_package( OpenCV REQUIRED )
find_package( OpenMP REQUIRED )

add_executable( silhouette src/algo.cpp
                           src/cli.cpp
                           src/cmd.cpp
                           src/io.cpp
                           src/main.cpp )

target_link_libraries( silhouette dlib::dlib
                                  stdc++fs
                                  ${OpenCV_LIBS} )
