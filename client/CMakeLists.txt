cmake_minimum_required(VERSION 3.2)

project(CarRacing VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if (UNIX)
	#add_definitions(-DUNIX)
	#add_compile_options("-std=c++17")
	add_compile_options("-Wall")
endif ()

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

set(MAIN_PROJECT "carRacing")

# Create libs
add_library(carRacingLib INTERFACE)
target_include_directories(carRacingLib INTERFACE lib/)

# Add subdirectories
#add_subdirectory(assets)
#add_subdirectory(lib)
add_subdirectory(src)
