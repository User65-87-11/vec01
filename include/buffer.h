#pragma once

#include "cglm/types.h"
#include <glad/gl.h>
#include <stdio.h>


typedef unsigned int ui32;



void buffer_describe_vec2(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);
void buffer_describe_vec3(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);
void buffer_describe_vec4(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);
void buffer_describe_mat4(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);
 
void buffer_describe_int(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);
void buffer_describe_uint(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);
void buffer_describe_float(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos);

void buffer_set_data(ui32 buffer_id, ui32 target,ui32 size, void  * data, ui32 gl_type);
void buffer_set_subdata(ui32 buffer_id,ui32 target ,ui32 size, void  * data, ui32 offest);
void buffer_set_data_a(ui32 array_id,ui32 buffer_id, ui32 target,ui32 size, void  * data, ui32 gl_type);
void buffer_set_subdata_a(ui32 array_id,ui32 buffer_id,ui32 target ,ui32 size, void  * data, ui32 offest);


#define buffer_describe(T) buffer_describe_ ## T
 
#ifdef BUFFER_IMPLEMENTATION

 
void buffer_describe_vec3(ui32 buffer_id, ui32 target,  ui32 att_div, ui32 att_pos){

	glBindBuffer(target, buffer_id);

	glVertexAttribPointer(att_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(att_pos); 
	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
	}
}
void buffer_describe_float(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos){
	glBindBuffer(target, buffer_id);

	glVertexAttribPointer(att_pos, 1, GL_FLOAT,  GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(att_pos); 
	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
	}
}
void buffer_describe_uint(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos){
	glBindBuffer(target, buffer_id);

	glVertexAttribIPointer(att_pos, 1, GL_UNSIGNED_INT,   sizeof(unsigned int), (void*)0);
	glEnableVertexAttribArray(att_pos); 
	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
	}
}
void buffer_describe_int(ui32 buffer_id, ui32 target, ui32 att_div, ui32 att_pos){
	glBindBuffer(target, buffer_id);

	glVertexAttribIPointer(att_pos, 1, GL_INT,   sizeof(int), (void*)0);
	glEnableVertexAttribArray(att_pos); 
	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
	}
}
void buffer_describe_vec4(ui32 buffer_id, ui32 target,  ui32 att_div, ui32 att_pos){

	glBindBuffer(target, buffer_id);

	glVertexAttribPointer(att_pos, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(att_pos); 
	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
	}
}
void buffer_describe_vec2(ui32 buffer_id, ui32 target,  ui32 att_div, ui32 att_pos){

	glBindBuffer(target, buffer_id);

	glVertexAttribPointer(att_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(att_pos); 
	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
	}
}

 
 
void buffer_describe_mat4(ui32 buffer_id, ui32 target,   ui32 att_div, ui32 att_pos){

	glBindBuffer(target, buffer_id);
 
	glEnableVertexAttribArray(att_pos); 
    glVertexAttribPointer(att_pos, 4, GL_FLOAT, GL_FALSE,  sizeof(float) * 16, (void*)0);

    glEnableVertexAttribArray(att_pos+1); 
    glVertexAttribPointer(att_pos+1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(1 *  sizeof(float)*4));

    glEnableVertexAttribArray(att_pos+2); 
    glVertexAttribPointer(att_pos+2, 4, GL_FLOAT, GL_FALSE,  sizeof(float) * 16, (void*)(2 * sizeof(float)*4));

    glEnableVertexAttribArray(att_pos+3); 
    glVertexAttribPointer(att_pos+3, 4, GL_FLOAT, GL_FALSE,  sizeof(float) * 16, (void*)(3 * sizeof(float)*4));
	

	

	if(att_div > 0)
	{
		glVertexAttribDivisor(att_pos,att_div);
		glVertexAttribDivisor(att_pos+1,att_div);
		glVertexAttribDivisor(att_pos+2,att_div);
		glVertexAttribDivisor(att_pos+3,att_div);
	 
	}
}
void buffer_set_subdata(ui32 buffer_id,ui32 target ,ui32 size, void  * data, ui32 offest){

	/**
	
void glBufferSubData(	GLenum target,
 	GLintptr offset,
 	GLsizeiptr size,
 	const void * data);

	*/
	glBindBuffer(target, buffer_id);
	glBufferSubData(target,  offest , size , data);
}
void buffer_set_data(ui32 buffer_id, ui32 target,ui32 size, void  * data, ui32 gl_type){
	glBindBuffer(target, buffer_id);
	glBufferData(target,  size , data, gl_type);
}

void buffer_set_subdata_a(ui32 arr_id,ui32 buffer_id,ui32 target ,ui32 size, void  * data, ui32 offest){

	/**
	
void glBufferSubData(	GLenum target,
 	GLintptr offset,
 	GLsizeiptr size,
 	const void * data);

	*/
	glBindVertexArray(arr_id);
	glBindBuffer(target, buffer_id);
	glBufferSubData(target,  offest , size , data);
	glBindVertexArray(0);
}
void buffer_set_data_a(ui32 arr_id,ui32 buffer_id, ui32 target,ui32 size, void  * data, ui32 gl_type){
	glBindVertexArray(arr_id);
	glBindBuffer(target, buffer_id);
	glBufferData(target,  size , data, gl_type);
	glBindVertexArray(0);
}

 
#endif

