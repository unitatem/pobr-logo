cmake_minimum_required(VERSION 3.10)
project(pobr_logo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_library(stdc++fs UNKNOWN IMPORTED)
set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a")

set(SOURCE_FILES
        src/main.cpp src/logger.h src/Morphology.cpp src/Morphology.h src/ConvolutionMask.cpp src/ConvolutionMask.h src/Segment.cpp src/Segment.h src/Utils.cpp src/Utils.h src/DetectedObject.cpp src/DetectedObject.h src/Detector.cpp src/Detector.h)

add_executable(pobr_logo ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} stdc++fs)
