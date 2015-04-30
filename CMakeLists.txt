cmake_minimum_required(VERSION 3.2)
project(image_to_sketch)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

include_directories(
    ${PROJECT_SOURCE_DIR}/source
    ${PROJECT_SOURCE_DIR}/../opencv-3.0.0/build/include
    ${PROJECT_SOURCE_DIR}/../boost-1.58.0
    ${PROJECT_SOURCE_DIR}/../tbb43_20150424oss/include
    ${PROJECT_SOURCE_DIR}/../rapidjson-1.0.1/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/../opencv-3.0.0/build/x86/mingw/bin/bin
    ${PROJECT_SOURCE_DIR}/../tbb43_20150424oss/build/windows_ia32_gcc_mingw4.8.1_release
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    source/main.cpp
    source/helper/color.hpp
    source/helper/color.cpp
    source/reducer/color_reducer.hpp
    source/reducer/color_reducer.cpp
)

add_executable(image_to_sketch ${SOURCE_FILES} )
target_link_libraries(image_to_sketch opencv_core300 opencv_imgproc300 opencv_imgcodecs300 tbb)