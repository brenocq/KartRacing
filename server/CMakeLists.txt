cmake_minimum_required(VERSION 3.2)

project(KartRacing VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if (UNIX)
    #add_definitions(-DUNIX)
    #add_compile_options("-std=c++17")
    add_compile_options("-Wall")    
endif ()

find_package(nlohmann_json CONFIG REQUIRED)

set(MAIN_PROJECT "kartRacing")
  
# Create libs              
add_library(kartRacingLib INTERFACE)
target_include_directories(kartRacingLib INTERFACE lib/)
  
# Add subdirectories       
add_subdirectory(src)      
