#pragma  once

 
 
#include "cglm/types.h"
#include <glad/gl.h>


typedef struct  Model_square Model_square;

typedef struct Model_square_inst{
	Model_square * ref;
	int idx;
}Model_square_inst;

typedef struct Model_square{
	size_t num_inst;
	GLuint VAO;
	GLuint VBO_POS,VBO_COL,VBO_TRANS;

	unsigned int * inst_ids;
 
	vec4 * center_coords;
	 
 
	vec3 * rotates;
	vec4 * colors;
	mat4 * transforms;

	

}Model_square;


void model_square_init(Model_square * model[static 1],size_t num_inst);
void model_square_move(Model_square_inst  inst[static 1],float x, float y);
void model_square_free(Model_square * model[static 1]);
Model_square_inst model_square_inst(Model_square  model[static 1],int  idx);
void model_square_update(Model_square [static 1]);

#ifdef MODEL_SQUARE_IMPLEMENTATION
 
#include <assert.h> 
#include <stddef.h>
#include <stdlib.h>
#include "buffer.h"
#include "cglm/affine.h"
#include "cglm/affine-pre.h"
#include "cglm/mat4.h"
#include "wavefront.h"
#include "camera.h"
#include <stdio.h>
 
#include <limits.h>

 

extern Camera * camera;

Model_square_inst model_square_inst(Model_square   model[static 1],int  idx){
	Model_square_inst ret;
	ret.ref = model;
	ret.idx = idx;
	return ret;
}
void model_square_update(Model_square model[static 1]){


	glm_mat4_identity_array(model->transforms, model->num_inst);
	for(int i=0;i<model->num_inst;i++)
	{
	 
		glm_rotate_x(model->transforms[i], model->rotates[i][0], model->transforms[i]);
		glm_rotate_y(model->transforms[i], model->rotates[i][1], model->transforms[i]);
		glm_rotate_z(model->transforms[i], model->rotates[i][2], model->transforms[i]);
		float * c = model->center_coords[i];
	 
		 
		//printf("%f, %f, %f, %f \n",c[0],c[1],c[2],c[3]);
		glm_translate(model->transforms[i], c);
	}


	buffer_set_subdata(model->VBO_TRANS, 
		GL_ARRAY_BUFFER,
		model->num_inst * sizeof(mat4), 
		model->transforms, 
		0);
}
void model_square_move(Model_square_inst  inst[static 1],float x, float y){
	Model_square *ref = inst->ref;
	vec4 v ={x,y,0,1};
	
	float *f = (float *)ref->center_coords[inst->idx];
	f[0] =x;
	f[1] =y;
	f[2] =0;
	f[3] =1.0;
	// glm_translate((vec4 *)&ref->transforms[inst->idx], v);
	// mat4 *m =  &ref->transforms[inst->idx];

	// if((*m)[0][3] > camera->screen_w   )
	// {
	// 	(*m)[0][3] = camera->screen_w;
	// }else if((*m)[0][3] < - camera->screen_w )
	// {
	// 	(*m)[0][3] = - camera->screen_w;
	// }

	// if((*m)[1][3] > camera->screen_h   )
	// {
	// 	(*m)[1][3] = camera->screen_h;
	// }else if((*m)[1][3] < - camera->screen_h )
	// {
	// 	(*m)[1][3] = - camera->screen_h;
	// }

}
void model_square_init(Model_square * model[static 1],size_t num_inst){
	* model = calloc(1, sizeof(Model_square));
	assert(*model);

	Model_square * ref = *model;
	ref->num_inst = num_inst;
	ref->center_coords = calloc(num_inst, sizeof(vec4));
	ref->rotates = calloc(num_inst, sizeof(vec3));
	ref->colors = calloc(num_inst, sizeof(vec4));
	ref->transforms = calloc(num_inst, sizeof(mat4));
	ref->inst_ids = calloc(num_inst, sizeof(unsigned int));
	
	glGenVertexArrays(1,&ref->VAO);
	glGenBuffers(1,&ref->VBO_POS);
	glGenBuffers(1,&ref->VBO_COL);
	glGenBuffers(1,&ref->VBO_TRANS);


	for(size_t i=0;i<num_inst;i++)
	{
		//float * f = (float*)&ref->center_coords[i];
	 
		glm_mat4_identity((vec4*)&ref->transforms[i]);
		ref->inst_ids[i] = 0xff000000 + i;

		float * c = (float*)&ref->colors[i];
		c[0]= 1.0f;
		c[1]= 0.5f;
		c[2]= 0.2f;
		c[3]= 1.0f;
	}

 


	WF * wf;
	wf_load(&wf, "../assets/models/square2d.obj");

	float * vpos ;
	size_t size;
	wf_get_vert_data( wf, &vpos, &size);

	glBindVertexArray(ref->VAO);


 
 

	buffer_describe(vec3)(ref->VBO_POS,GL_ARRAY_BUFFER, 0, 0);
	buffer_set_data(ref->VBO_POS, GL_ARRAY_BUFFER, size, vpos, GL_STATIC_DRAW);

	buffer_describe(vec4)(ref->VBO_COL,GL_ARRAY_BUFFER,1, 1);
	buffer_set_data(ref->VBO_COL, GL_ARRAY_BUFFER, ref->num_inst * sizeof(vec4), ref->colors, GL_DYNAMIC_DRAW);

	buffer_describe(mat4)(ref->VBO_TRANS,GL_ARRAY_BUFFER, 1, 10);
	buffer_set_data(ref->VBO_TRANS, GL_ARRAY_BUFFER, ref->num_inst * sizeof(mat4), ref->transforms, GL_DYNAMIC_DRAW);
	glBindVertexArray(0);


	free(vpos);
}
void model_square_free(Model_square * model[static 1]){
	assert(*model);
	free((*model)->colors);
	free((*model)->center_coords);
	free((*model)->rotates);
	free((*model)->transforms);
	free((*model)->inst_ids);
	free(*model);
}
 
#endif