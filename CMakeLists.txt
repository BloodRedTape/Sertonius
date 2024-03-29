cmake_minimum_required(VERSION 3.10)

project(Sample)

set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/StraitX-Base)
add_subdirectory(libs/StraitX-ImGui)
add_subdirectory(libs/Assimp-FBXLoader)

set(SAMPLE_SOURCES
    "sources/main.cpp"
    "sources/render/mesh.cpp"
    "sources/engine/engine.cpp" 
    "sources/render/renderer3d.cpp" 
    "sources/render/passes/geometry_pass.cpp" 
    "sources/render/render_targets.cpp" 
    "sources/render/passes/composite_pass.hpp" 
    "sources/render/passes/composite_pass.cpp"
    "sources/framework/actor.cpp" 
    "sources/framework/world.cpp" 
    "sources/framework/actor_component.cpp" 
    "sources/framework/object.cpp"   
    "sources/components/mesh_component.cpp"  
    "sources/sertonius/player.cpp" 
    "sources/sertonius/sertonius_game_mode.cpp" 
    "sources/render/camera.cpp"
    "sources/components/camera_component.cpp" 
    "sources/core/log.cpp" 
    "sources/engine/game_engine.cpp" 
    "sources/engine/editor_engine.cpp" 
    "sources/framework/pawn.cpp"
    "sources/render/scene.cpp"
    "sources/core/transform.cpp" 
    "sources/framework/assets_manager.cpp"
    "sources/framework/actor_loader.cpp"
    "sources/render/passes/lighting_pass.cpp"
    "sources/render/hot_reloader.cpp" 
    "sources/render/common.cpp" 
)

set(SAMPLE_INCLUDE
    "./sources"
)

add_executable(Sample ${SAMPLE_SOURCES})
target_link_libraries(Sample StraitXBase StraitXImGui assimp-fbxloader)
target_include_directories(Sample
    PUBLIC ${SAMPLE_INCLUDE}
)

target_compile_features(Sample PRIVATE cxx_std_17)