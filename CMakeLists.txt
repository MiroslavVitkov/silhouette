project( silhouette )

cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options( -Wall -Wextra -pedantic -Werror -Wfatal-errors )
find_package( OpenCV REQUIRED )
find_package( ZLIB REQUIRED )
find_library( ZLIBCOMPLETE
              NAMES zlibcomplete libzlibcomplete.a
              HINTS "/usr/local/lib/" )

add_executable( silhouette src/algo.cpp
                           src/cli.cpp
                           src/cmd.cpp
                           src/io.cpp
                           src/main.cpp )

target_link_libraries( silhouette stdc++fs
                                  ${OpenCV_LIBS}
                                  ${ZLIBCOMPLETE}
                                  ${ZLIB_LIBRARIES} )
target_include_directories( silhouette SYSTEM PRIVATE "/home/vorac/proj/cppcodec" )
target_include_directories( silhouette SYSTEM PRIVATE "/home/vorac/proj/json/include/" )
target_include_directories( silhouette SYSTEM PRIVATE "/home/vorac/proj/silhouette/ext" )
target_include_directories( silhouette SYSTEM PRIVATE "/home/vorac/proj/zlibcomplete/lib" )
