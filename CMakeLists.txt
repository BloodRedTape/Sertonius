cmake_minimum_required(VERSION 3.10)

project(Sample)

set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/StraitX-Base)

set(SAMPLE_SOURCES
    "sources/main.cpp"
 "sources/application.cpp" "sources/render/renderer.cpp")

set(SAMPLE_INCLUDE
    "./sources"
)

add_executable(Sample ${SAMPLE_SOURCES})
target_link_libraries(Sample StraitXBase)
target_include_directories(Sample
    PUBLIC ${SAMPLE_INCLUDE}
)