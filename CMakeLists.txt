cmake_minimum_required(VERSION 3.10)

project(Sample)

set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/StraitX-Base)
add_subdirectory(libs/Assimp-FBXLoader)

set(SAMPLE_SOURCES
    "sources/main.cpp"
    "sources/render/mesh.cpp"
 "sources/application.cpp" "sources/render/renderer.cpp" "sources/render/passes/geometry_pass.cpp" "sources/render/render_targets.cpp" "sources/render/passes/composite_pass.hpp" "sources/render/passes/composite_pass.cpp" "sources/framework/actor.cpp" "sources/framework/world.cpp" "sources/framework/actor_component.cpp" "sources/framework/object.cpp"   "sources/components/mesh_component.cpp")

set(SAMPLE_INCLUDE
    "./sources"
)

add_executable(Sample ${SAMPLE_SOURCES})
target_link_libraries(Sample StraitXBase assimp-fbxloader)
target_include_directories(Sample
    PUBLIC ${SAMPLE_INCLUDE}
)