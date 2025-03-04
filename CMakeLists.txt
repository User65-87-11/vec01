cmake_minimum_required(VERSION 3.12)

#set(CMAKE_C_STANDARD 23) 
set(CMAKE_C_COMPILER "C:/clang+llvm-19.1.0-x86_64-pc-windows-msvc/bin/clang.exe")
#set(CMAKE_CXX_COMPILER "C:/clang+llvm-19.1.0-x86_64-pc-windows-msvc/bin/clang++.exe")
#one compiler is enough

project(vec01 C)


message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
 
 

add_compile_options( 
	-std=c23 
	-Wno-format 
	-Wno-incompatible-pointer-types-discards-qualifiers
 
	)


add_subdirectory(external/glad)
add_subdirectory(main)
add_subdirectory(src)
 

#ENV CMAKE_GENERATOR Ninja

#cmake runs a generator , generator goes cl.exe if microsofts
 
 

 