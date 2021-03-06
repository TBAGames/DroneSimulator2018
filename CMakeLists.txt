cmake_minimum_required(VERSION 2.6)

# Name of project
project(DroneSimulator2018)

# Specify project files: header files and source files
set(HDRS
    asteroid.h camera.h game.h model_loader.h resource.h resource_manager.h scene_graph.h scene_node.h settings.h Bullet.h Bomb.h Rocket.h
)
 
set(SRCS
    asteroid.cpp camera.cpp game.cpp main.cpp resource.cpp resource_manager.cpp scene_graph.cpp scene_node.cpp settings.cpp Bullet.cpp Bomb.cpp Rocket.cpp material_fp.glsl material_vp.glsl metal_fp.glsl metal_vp.glsl plastic_fp.glsl plastic_vp.glsl textured_material_fp.glsl textured_material_vp.glsl three-term_shiny_blue_fp.glsl three-term_shiny_blue_vp.glsl normal_map_vp.glsl normal_map_fp.glsl screen_space_vp.glsl screen_space_fp.glsl particle_vp.glsl particle_fp.glsl particle_gp.glsl flame_vp.glsl flame_fp.glsl flame_gp.glsl Enemy.cpp Enemy.h
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(DroneSimulator2018 ${HDRS} ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(DroneSimulator2018 ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(DroneSimulator2018 ${GLEW_LIBRARY})
target_link_libraries(DroneSimulator2018 ${GLFW_LIBRARY})
target_link_libraries(DroneSimulator2018 ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(DroneSimulator2018 PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
