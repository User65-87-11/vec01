cmake_minimum_required(VERSION 3.12)
project(Hello)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_executable(main)
 
target_sources(
    main PRIVATE
    main.c
)

# file(GLOB SRC_FILES ../src/*.c)

# target_sources(
# 	main PRIVATE
# 	${SRC_FILES}
# )
 
target_include_directories(
    main PRIVATE 
    C:/glfw-3.4.bin.WIN64/include
    C:/glad/include
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/include/
 
)

#target_link_libraries(main mylib)

 
target_link_libraries(
    main PRIVATE
    C:/glfw-3.4.bin.WIN64/lib-vc2022/glfw3_mt.lib
    glad
	#"../../libs/freetype/vs2015-2022/win64/freetype"
)
 