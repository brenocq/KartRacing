set(exe_name ${MAIN_PROJECT})
                           
set(src_files              
    main.cpp
    server.cpp
) 
  
set(src_files_helpers      
    helpers/log.cpp
) 
  
add_executable(${exe_name} 
    ${src_files}           
    ${src_files_helpers}   
) 
  
target_link_libraries(${exe_name} PRIVATE nlohmann_json nlohmann_json::nlohmann_json kartRacingLib)
target_include_directories(${exe_name} PRIVATE ${STB_INCLUDE_DIRS})
