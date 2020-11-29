set(exe_name ${MAIN_PROJECT})

set(src_files
	main.cpp
	application.cpp
	window.cpp
	shader.cpp
	mesh.cpp
	texture.cpp
	cubemap.cpp
	cubeMesh.cpp
	camera.cpp
	userInterface.cpp
	client.cpp

	kart.cpp
	garage.cpp
	speedway.cpp
)

set(src_files_helpers
	helpers/log.cpp
	helpers/common.cpp
)

add_executable(${exe_name} 
	${src_files} 
	${src_files_helpers} 
)

target_link_libraries(${exe_name} PRIVATE GLEW::GLEW glfw glm nlohmann_json nlohmann_json::nlohmann_json kartRacingLib)
target_include_directories(${exe_name} PRIVATE ${STB_INCLUDE_DIRS})

add_subdirectory(shaders)
add_dependencies(${exe_name} shaders assets)

