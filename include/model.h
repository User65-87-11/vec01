#pragma  once



#include <glad/gl.h>
#include <math.h>
#include "cglm/affine.h"
#include "cglm/io.h"
#include "cglm/types.h"
#include "list.h"

typedef struct  Model_circle Model_circle;
typedef struct  Model_rect Model_rect;
typedef struct  Model_line Model_line;
typedef struct  Model Model;
 
 

typedef struct Model{
	size_t num_verts;
	GLuint VAO;
	GLuint VBO_POS,VBO_COL,VBO_TRANS;

	int max_inst;
	int num_inst;
	int seg_num;
	Model_circle * circles;
 
	mat4 * transforms;
	vec4 * colors;


	bool added;

}Model;

typedef struct Model_circle{
	Model * ref;
	int inst_id;
	float x,y,r;
	 

}Model_circle;

void model_init(Model * model[static 1] );
 
void model_free(Model * model[static 1]);



void model_rect_add(Model  model[static 1],float cx, float cy, float rclose, float rfar);
void model_line_add(Model  model[static 1],float ax, float ay, float bx, float by);
void model_circle_add(Model  model[static 1],float cx, float cy, float r);
void model_circle_add3(Model  model[static 1],float *xyr);
void model_circle_delete(Model  model[static 1],int idx);

Model_circle *model_circle_ret(Model model[static 1],int ind);
void model_circle_move_to(Model_circle model[static 1],float cx, float cy);
void model_circle_radius(Model_circle model[static 1],float r);
void model_circle_rotate(Model_circle model[static 1],float rad);

void model_update(Model [static 1]);
 


#ifdef MODEL_LINE_IMPLEMENTATION
#include "camera.h"
#include "cglm/affine-pre.h"
#include "cglm/mat4.h"
#include <assert.h> 
#include <stddef.h>
#include <stdlib.h>
#include "buffer.h"
#include "wavefront.h"

#include <stdio.h>
 
#include <limits.h>


 

extern Camera * camera;

Model_circle* model_circle_ret(Model model[static 1],int ind){
	assert(ind < model->num_inst);
	Model_circle *ret = &model->circles[ind];
	return  ret;
}
 
void model_update(Model model[static 1]){

	
	if(model->added)
	{
		model->added = false;
		for(int i=0;i<model->num_inst;i++){
			buffer_set_subdata(model->VBO_TRANS, GL_ARRAY_BUFFER,   sizeof(mat4) , &model->transforms[i],i *sizeof(mat4)  );
			printf("updated\n");
		}
	}
	

	
}
void model_rect_add(Model model[static 1],float cx, float cy, float rclose, float rfar){
	
}
void model_line_add(Model  model[static 1],float ax, float ay, float bx, float by){
	 
}
void model_circle_delete(Model  model[static 1],int idx){
	assert(model->num_inst > idx);
	if(idx +1 < model->num_inst)
	{
		//swap with last
		int last_idx = model->num_inst - 1;
		Model_circle c = model->circles[last_idx];
		c.inst_id = idx;
		mat4 *ct = &model->transforms[last_idx];
		vec4 *cc = &model->colors[last_idx];

		model->circles[idx] = c;
		glm_mat4_copy((vec4*)ct, (vec4*)&model->transforms[idx]);
		glm_vec4_copy((float*)cc, (float*)&model->colors[idx]);
		 

	}
	model->num_inst --;
}
void model_circle_add3(Model   model[static 1],float * xyr){
	 
	assert(model->num_inst < model->max_inst);
	 
	Model_circle * cir =  &model->circles[model->num_inst];
	cir->ref = model;
	cir->inst_id = model->num_inst  ;
	cir->r = xyr[3];
	cir->x = xyr[0];
	cir->y = xyr[1];

	model->num_inst = model->num_inst + 1;
	vec4 * t = (vec4*)&model->transforms[cir->inst_id];
	glm_mat4_identity(t);
	
 
	vec3 s = {xyr[3],xyr[3],0.0f};
	glm_scale(t, s);
	vec3 v={xyr[0],xyr[1],0};
	glm_translate(t, v);

	//glm_mat4_print(t, stdout);
 
	model->added = true;
}
 
void model_init(Model * model[static 1]){
	* model = calloc(1, sizeof(Model));
	assert(*model);

	Model * ref = *model;
 
	ref->added = false;
	ref->max_inst = 100;
	ref->num_inst = 0;
	ref->colors = calloc(1, sizeof(vec4));
	ref->circles = calloc(ref->max_inst, sizeof(Model_circle));
	ref->transforms = calloc(ref->max_inst, sizeof(mat4));
	ref->seg_num = 36; 
	glm_mat4_identity_array(ref->transforms,ref->max_inst);
	//ref->inst_ids = calloc(num_inst, sizeof(unsigned int));
	
	glGenVertexArrays(1,&ref->VAO);
	glGenBuffers(1,&ref->VBO_POS);
	glGenBuffers(1,&ref->VBO_COL);
	glGenBuffers(1,&ref->VBO_TRANS);

	float * c = (float*)&ref->colors[0];
	c[0]= 1.0f;
	c[1]= 0.5f;
	c[2]= 0.2f;
	c[3]= 1.0f;

	
	
 
	vec3 * points = malloc(sizeof(vec3[ref->seg_num]));
 
	double pi = 3.14159265359;
	float step = (360.0/ref->seg_num);
	for(int i=0;i<ref->seg_num;i++)
	{	
		
		
		float radian = i*step * (pi/180.0);

		float tx = cos(radian)  ;
		float ty = sin(radian)   ;
		
		float * pt = (float *)&points[i];
		pt[0] = tx;
		pt[1] = ty;
		pt[2] = 0;

	//	glm_vec3_print(pt, stdout);
		
	}
 
	glBindVertexArray(ref->VAO);
 
	buffer_describe(vec3)(ref->VBO_POS,GL_ARRAY_BUFFER, 0, 0);
	buffer_set_data(ref->VBO_POS, GL_ARRAY_BUFFER,   sizeof(vec3) * ref->seg_num, points, GL_STATIC_DRAW);

	buffer_describe(vec4)(ref->VBO_COL,GL_ARRAY_BUFFER,99999999, 1);
	buffer_set_data(ref->VBO_COL, GL_ARRAY_BUFFER,   sizeof(vec4), ref->colors, GL_DYNAMIC_DRAW);

	buffer_describe(mat4)(ref->VBO_TRANS,GL_ARRAY_BUFFER,1, 10);
	buffer_set_data(ref->VBO_TRANS, GL_ARRAY_BUFFER,   sizeof(mat4)*ref->max_inst, ref->transforms, GL_DYNAMIC_DRAW);
	
	glBindVertexArray(0);
	free(points);
 
}
void model_free(Model * model[static 1]){
	assert(*model);
	free((*model)->colors);
	free((*model)->transforms);
	free((*model)->circles);
 
	free(*model);
}
#endif