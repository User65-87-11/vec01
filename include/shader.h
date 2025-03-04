#pragma once



void shader_use( unsigned int ID) ;
 
void shader_setBool(const char * name, bool value)  ;
 
void shader_setInt(const char * name, int value)  ;
 
void shader_setFloat(const char * name, float value)  ;

void shader_setVec2(const char * name, const float * value) ;
 
 
void shader_setVec3(const char * name, const  float * value)  ;
 
void shader_setVec4(const char * name, const  float *  value) ;
 
void shader_setMat2(const char * name, const  float *  mat) ;
 
void shader_setMat3(const char * name, const  float *  mat) ;
 
void shader_setMat4(const char * name, const  void * mat) ;
 
void shader_checkCompileErrors( unsigned int id , const char * type);

unsigned int shader_compile(const char* vertexPath, const char* fragmentPath);

void shader_bind_ubo(const char* block_name,unsigned int ubi_id);

void shader_bind_ubo_shader_bind(const char* block_name,unsigned int ubi_id, unsigned int shader_binding);

#ifdef SHADER_IMPLEMENTATION
#include "cglm/types.h"
#include <glad/gl.h>
#include <cglm/cglm.h>

#include <stdlib.h>
 

 

#include <stdio.h>
#include <string.h>
 
 
 
static unsigned int current_shader_id;

void shader_bind_ubo(const char* block_name,unsigned int ubi_id){

	unsigned int matrices_index = glGetUniformBlockIndex(current_shader_id, block_name); 
	glUniformBlockBinding(current_shader_id, matrices_index, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubi_id); 
}

void shader_bind_ubo_shader_bind(const char* block_name,unsigned int ubi_id, unsigned int shader_binding){

	unsigned int matrices_index = glGetUniformBlockIndex(current_shader_id, block_name); 
	glUniformBlockBinding(current_shader_id, matrices_index, shader_binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, shader_binding, ubi_id); 
}
unsigned int shader_compile(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath

	printf("V Path: %s \n",vertexPath);
	printf("F Path: %s \n",fragmentPath);
	 

	FILE *vertex_file;
	FILE *fragment_file;

 
	char * vertex_data = nullptr;
	char * fragment_data = nullptr;
	{
		
		int err = fopen_s(&vertex_file,vertexPath, "rb");
		if(err)
		{
			printf("V open error: %08x \n",err);
			return -1;
		}
		fseek(vertex_file, 0, SEEK_END);

		long num = ftell(vertex_file);
	 
		rewind(vertex_file);
		

		vertex_data = malloc(num + 1);
		vertex_data[num] = '\0';
		num = fread(vertex_data, sizeof(char), num,vertex_file );
	 
		fclose(vertex_file);

		err = fopen_s(&fragment_file,fragmentPath, "rb");
		if(err)
		{
			printf("F open error: %08x \n",err);
			return -1;
		}
		fseek(fragment_file, 0, SEEK_END);
		num = ftell(fragment_file);
	 
		rewind(fragment_file);
		fragment_data = malloc(num +1);
		fragment_data[num] = '\0';
		num = fread(fragment_data, sizeof(char), num,fragment_file );
	 
		fclose(fragment_file);
 
	}
	 
	// const char* vShaderCode = vertex_data;
	// const char * fShaderCode = fragment_data;

	//std::cout << vShaderCode << std::endl;
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_data, NULL);
	glCompileShader(vertex);
	shader_checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_data, NULL);
	glCompileShader(fragment);
	shader_checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	unsigned int ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	shader_checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	free(vertex_data);
	free(fragment_data);
	return ID;
}


void shader_use( unsigned int ID ) 
{ 
	glUseProgram(ID); 
	current_shader_id = ID;
}
// utility uniform functions
// ------------------------------------------------------------------------
void shader_setBool(const char * name, bool value)  
{         
	glUniform1i(glGetUniformLocation(current_shader_id, name), (int)value); 
}
// ------------------------------------------------------------------------
void shader_setInt( const char * name, int value)  
{ 
	glUniform1i(glGetUniformLocation(current_shader_id, name ), value); 
}
// ------------------------------------------------------------------------
void shader_setFloat( const char * name, float value)  
{ 
	 
	glUniform1f(glGetUniformLocation(current_shader_id, name), value); 
}
void shader_setiVec2( const char * name, const  int *  value)  
{ 
	//vec2 * inst = (vec2 * )value;
	glUniform2iv(glGetUniformLocation(current_shader_id, name), 1, value); 
}
 
void shader_setVec2( const char * name, const  float *  value)  
{ 
	//vec2 * inst = (vec2 * )value;
	glUniform2fv(glGetUniformLocation(current_shader_id, name), 1, value); 
}
 
// ------------------------------------------------------------------------
void shader_setVec3( const char * name, const   float *  value)  
{ 
	//vec3 * inst = (vec3 * )value;
	glUniform3fv(glGetUniformLocation(current_shader_id, name), 1, value); 
}
 
// ------------------------------------------------------------------------
void shader_setVec4( const char * name, const   float *   value)  
{ 
 
	glUniform4fv(glGetUniformLocation(current_shader_id, name), 1, value); 
}
 
// ------------------------------------------------------------------------
void shader_setMat2( const char * name, const  float *  mat)  
{
	 
	glUniformMatrix2fv(glGetUniformLocation(current_shader_id, name), 1, GL_FALSE,   mat );
}
// ------------------------------------------------------------------------
void shader_setMat3( const char * name, const   float *   mat)  
{
	//mat3 * inst = (mat3 * )mat;
	glUniformMatrix3fv(glGetUniformLocation(current_shader_id, name), 1, GL_FALSE, mat);
}
// ------------------------------------------------------------------------
void shader_setMat4( const char * name, const   void *  mat)  
{
	 
	glUniformMatrix4fv(glGetUniformLocation(current_shader_id, name), 1, GL_FALSE, mat);
}

 
// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void shader_checkCompileErrors(unsigned int id, const char type[static 1])
{
	 
	int success;
	char infoLog[1024];
	if (strcmp(type,"PROGRAM") !=0 )
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 1024, NULL, infoLog);

			printf("%s %s %s %s\n",
				"ERROR::SHADER_COMPILATION_ERROR of type: ",
				type,
				infoLog,
				"\n -- --------------------------------------------------- -- "
			);
	 
		}
	}
	else
	{
		glGetProgramiv(current_shader_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			int len  =0;
			glGetProgramInfoLog(current_shader_id, 1024, &len, infoLog);


			printf("%s %s %s %s, len:%i\n",
				"ERROR::PROGRAM_LINKING_ERROR of type: ",
				type,
				infoLog,
				len,
				"\n -- --------------------------------------------------- -- "
			);
			 
			 
		}
	}
}

#endif 