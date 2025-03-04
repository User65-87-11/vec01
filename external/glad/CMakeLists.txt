cmake_minimum_required(VERSION 3.12)
project(Hello)

 


# compile glad as target
add_library(glad C:/glad/src/gl.c)

target_include_directories(
    glad PRIVATE 
    C:/glad/include
)

 