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
 
void model_init(Model * model[static 1] );
 
void model_free(Model * model[static 1]);



void model_rect_add(Model  * model ,float cx, float cy, float r);
void model_line_add(Model * model,float ax, float ay, float bx, float by);
void model_circle_add(Model  * model,float cx, float cy, float r);
 
void model_circle_delete(Model  * model,int idx);

Model_circle *model_circle_ret(Model * model,int ind);
void model_circle_move_to(Model_circle * model,float cx, float cy);
void model_circle_radius(Model_circle * model,float r);
void model_circle_rotate(Model_circle* model,float rad);

void model_update(Model * model);
  

typedef struct Model{
	size_t num_verts;
	GLuint VAO_CIRCLE,VAO_RECT,VAO_LINE;
	GLuint VBO_CIRCLE_COL,VBO_CIRCLE_TRANS,VBO_CIRCLE_POS;
	GLuint VBO_LINE_COL,VBO_LINE_POS,VBO_LINE_TRANS;
	GLuint VBO_RECT_COL,VBO_RECT_TRANS,VBO_RECT_POS;

	int max_inst;

	int num_inst_circle;
	int num_inst_rect;
	int num_inst_line;

	int point_num_circ;
	int point_num_rect;
	int point_num_line;

	Model_circle * circles;
	Model_rect * rects;
	Model_line * lines;
	
	mat4 * transforms_circle;
	mat4 * transforms_rect;
	mat4 * transforms_line;

	vec4 * colors_rect;
	vec4 * colors_circle;
	vec4 * colors_line;


	bool update_circ;
	bool update_rect;
	bool update_line;

}Model;

typedef struct Model_circle{
	Model * ref;
	int inst_id;
	float x,y,r;
	 

}Model_circle;
typedef struct Model_rect{
	Model * ref;
	int inst_id;
	float x,y,r,r2;
	 

}Model_rect;

typedef struct Model_line{
	Model * ref;
	int inst_id;
	vec3 p[2];
	
	 

}Model_line;


#ifdef MODEL_IMPLEMENTATION
#include "camera.h"
#include "cglm/affine.h"
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
	assert(ind < model->num_inst_circle);
	Model_circle *ret = &model->circles[ind];
	return  ret;
}
 
void model_update(Model model[static 1]){

	
	if(model->update_circ)
	{
		model->update_circ = false;

		for(int i=0;i<model->num_inst_circle;i++){
			buffer_set_subdata(model->VBO_CIRCLE_TRANS, 
				GL_ARRAY_BUFFER,   
				sizeof(mat4) , 
				&model->transforms_circle[i],
				i *sizeof(mat4) );
			
		}

		printf("updated circ\n");
	}
	

	if(model->update_line)
	{
		model->update_line = false;

		for(int i=0;i<model->num_inst_line;i++){
			buffer_set_subdata(model->VBO_LINE_POS, 
				GL_ARRAY_BUFFER,   
				sizeof(vec3)*2 , 
				model->lines[i].p,
				i *sizeof(vec3)*2 );
			
		}

		printf("updated line\n");
	}

	if(model->update_rect)
	{
		model->update_rect = false;

		for(int i=0;i<model->num_inst_rect;i++){
			buffer_set_subdata(model->VBO_RECT_TRANS, 
				GL_ARRAY_BUFFER,   
				sizeof(mat4) , 
				&model->transforms_rect[i],
				i *sizeof(mat4) );
			
		}

		printf("updated rect\n");
	}
	
}
void model_rect_add(Model model[static 1],float cx, float cy, float r ){
	
	assert(model->num_inst_rect < model->max_inst);
	 
	Model_rect * rect =  &model->rects[model->num_inst_rect];
	rect->ref = model;
	rect->inst_id = model->num_inst_rect  ;
	rect->r = r;
 
	rect->x = cx;
	rect->y = cy;

	model->num_inst_rect = model->num_inst_rect + 1;
	vec4 * t = (vec4*)&model->transforms_rect[rect->inst_id];
	glm_mat4_identity(t);
	
 
	vec3 s = {r,r,0.0f};
	glm_scale(t, s);
	vec3 v={cx,cy,0};
	glm_translate(t, v);

	//glm_mat4_print(t, stdout);
 
	model->update_rect = true;
}
void model_line_add(Model  model[static 1],float ax, float ay, float bx, float by){
	 
	assert(model->num_inst_line < model->max_inst);
	 
	Model_line * cir =  &model->lines[model->num_inst_line];
	cir->ref = model;
	cir->inst_id = model->num_inst_line  ;
	cir->p[0][0] = ax;
	cir->p[0][1] = ay;
	cir->p[0][2] = 0;

	cir->p[1][0] = bx;
	cir->p[1][1] = by;
	cir->p[1][2] = 0;

	model->num_inst_line = model->num_inst_line + 1;

	//glm_mat4_print(t, stdout);
 
	model->update_circ = true;


}
void model_circle_delete(Model  model[static 1],int idx){
	assert(model->num_inst_circle > idx);
	if(idx +1 < model->num_inst_circle)
	{
		//swap with last
		int last_idx = model->num_inst_circle - 1;
		Model_circle c = model->circles[last_idx];
		c.inst_id = idx;
		mat4 *ct = &model->transforms_circle[last_idx];
		vec4 *cc = &model->colors_circle[last_idx];

		model->circles[idx] = c;
		glm_mat4_copy((vec4*)ct, (vec4*)&model->transforms_circle[idx]);
		glm_vec4_copy((float*)cc, (float*)&model->colors_circle[idx]);
		 


	}
	model->num_inst_circle --;
	model->update_circ = true;
}
void model_circle_add(Model  model[static 1],float cx, float cy, float r){
	 
	assert(model->num_inst_circle < model->max_inst);
	 
	Model_circle * cir =  &model->circles[model->num_inst_circle];
	cir->ref = model;
	cir->inst_id = model->num_inst_circle  ;
	cir->r = r;
	cir->x = cx;
	cir->y = cy;

	model->num_inst_circle = model->num_inst_circle + 1;
	vec4 * t = (vec4*)&model->transforms_circle[cir->inst_id];
	glm_mat4_identity(t);
	
 
	vec3 s = {r,r,0.0f};
	glm_scale(t, s);
	vec3 v={cx,cy,0};
	glm_translate(t, v);

	//glm_mat4_print(t, stdout);
 
	model->update_circ = true;
}
#define PI 3.14159265358979323846
 
void model_init(Model * model[static 1]){
	* model = calloc(1, sizeof(Model));
	assert(*model);

	Model * ref = *model;
 
	ref->update_circ = false;
	ref->update_line = false;
	ref->update_rect = false;

	ref->max_inst = 100;
 
	ref->colors_circle = calloc(1, sizeof(vec4));
	ref->colors_rect = calloc(1, sizeof(vec4));
	ref->colors_line = calloc(1, sizeof(vec4));


	ref->circles = calloc(ref->max_inst, sizeof(Model_circle));
	ref->rects = calloc(ref->max_inst, sizeof(Model_rect));
	ref->lines = calloc(ref->max_inst, sizeof(Model_line));

	ref->transforms_circle = calloc(ref->max_inst, sizeof(mat4));
	ref->transforms_rect = calloc(ref->max_inst, sizeof(mat4));
	ref->transforms_line = calloc(1, sizeof(mat4));

	ref->point_num_circ = 18; 
	ref->point_num_rect = 4;
	ref->point_num_line = 2;

	 
	glm_mat4_identity_array(ref->transforms_circle,ref->max_inst);
	glm_mat4_identity_array(ref->transforms_rect,ref->max_inst);
	//ref->inst_ids = calloc(num_inst, sizeof(unsigned int));
	
	glGenVertexArrays(1,&ref->VAO_CIRCLE);
	glGenVertexArrays(1,&ref->VAO_RECT);
	glGenVertexArrays(1,&ref->VAO_LINE);

	glGenBuffers(1,&ref->VBO_CIRCLE_POS);
	glGenBuffers(1,&ref->VBO_RECT_POS);
	glGenBuffers(1,&ref->VBO_LINE_POS);

	glGenBuffers(1,&ref->VBO_CIRCLE_COL);
	glGenBuffers(1,&ref->VBO_RECT_COL);
	glGenBuffers(1,&ref->VBO_LINE_COL);

	glGenBuffers(1,&ref->VBO_CIRCLE_TRANS);
	glGenBuffers(1,&ref->VBO_RECT_TRANS);
	glGenBuffers(1,&ref->VBO_LINE_TRANS);
	
 
 

	float * c = (float*)&ref->colors_circle[0];
	c[0]= 1.0f;
	c[1]= 0.5f;
	c[2]= 0.2f;
	c[3]= 1.0f;

	c = (float*)&ref->colors_line[0];
	c[0]= 1.0f;
	c[1]= 0.2f;
	c[2]= 0.5f;
	c[3]= 1.0f;

	c = (float*)&ref->colors_rect[0];
	c[0]= 0.5f;
	c[1]= 0.2f;
	c[2]= 1.9f;
	c[3]= 1.0f;

 
	vec3 * points_circle = malloc(sizeof(vec3[ref->point_num_circ]));

	float step = (360.0/ref->point_num_circ);
	float rad = (PI/180.0);
	for(int i=0;i<ref->point_num_circ;i++)
	{	
 
		float ang = i *step *rad;
		float tx =  cos(ang);
		float ty =  sin(ang);

		float * pt = (float *)&points_circle[i];
		pt[0] = tx;
		pt[1] = ty;
		pt[2] = 0;

	}

 
	vec3 * points_sqare = malloc(sizeof(vec3[ref->point_num_rect]));
	float * pt = (float *)&points_sqare[0];
	pt[0] = 1.0;pt[1] = 1.0;pt[2] = 0;

	pt = (float *)&points_sqare[1];
	pt[0] = 1.0;pt[1] = -1.0;pt[2] = 0;

	pt = (float *)&points_sqare[2];
	pt[0] = -1.0;pt[1] = -1.0;pt[2] = 0;

	pt = (float *)&points_sqare[3];
	pt[0] = -1.0;pt[1] = 1.0;pt[2] = 0;


 
	glm_mat4_identity((vec4*)ref->transforms_line);



	//--circle
	glBindVertexArray(ref->VAO_CIRCLE);
 

	buffer_describe(vec3)(ref->VBO_CIRCLE_POS,GL_ARRAY_BUFFER, 0, 0);
	buffer_set_data(ref->VBO_CIRCLE_POS, 
		GL_ARRAY_BUFFER,   
		sizeof(vec3) * ref->point_num_circ, 
		points_circle, 
		GL_STATIC_DRAW);

	buffer_describe(vec4)(ref->VBO_CIRCLE_COL,GL_ARRAY_BUFFER,99999999, 1);
	buffer_set_data(ref->VBO_CIRCLE_COL, 
		GL_ARRAY_BUFFER,   
		sizeof(vec4), 
		ref->colors_circle, 
		GL_STATIC_DRAW);

	buffer_describe(mat4)(ref->VBO_CIRCLE_TRANS,GL_ARRAY_BUFFER,1, 10);
	buffer_set_data(ref->VBO_CIRCLE_TRANS, 
		GL_ARRAY_BUFFER,   
		sizeof(mat4)*ref->max_inst, 
		ref->transforms_circle, 
		GL_DYNAMIC_DRAW);
	


	//-- rect
	glBindVertexArray(ref->VAO_RECT);
		
	buffer_describe(vec3)(ref->VBO_RECT_POS,GL_ARRAY_BUFFER, 0, 0);
	buffer_set_data(ref->VBO_RECT_POS, 
		GL_ARRAY_BUFFER,  
		sizeof(vec3) * ref->point_num_rect, 
		points_sqare, 
		GL_STATIC_DRAW);


	buffer_describe(mat4)(ref->VBO_RECT_TRANS,GL_ARRAY_BUFFER,1, 10);
	buffer_set_data(ref->VBO_RECT_TRANS, 
		GL_ARRAY_BUFFER,   
		sizeof(mat4)*ref->max_inst, 
		ref->transforms_rect, 
		GL_DYNAMIC_DRAW);


	buffer_describe(vec4)(ref->VBO_RECT_COL,GL_ARRAY_BUFFER,99999999, 1);
	buffer_set_data(ref->VBO_RECT_COL, 
		GL_ARRAY_BUFFER,   
		sizeof(vec4), 
		ref->colors_rect, 
		GL_STATIC_DRAW);

	
	//-- line
	glBindVertexArray(ref->VAO_LINE);
	buffer_describe(vec3)(ref->VBO_LINE_POS,GL_ARRAY_BUFFER, 1, 0);
	buffer_set_data(ref->VBO_LINE_POS, 
		GL_ARRAY_BUFFER,   
		sizeof(vec3[2]) * ref->max_inst, 
		nullptr, 
		GL_DYNAMIC_DRAW);

	buffer_describe(vec4)(ref->VBO_LINE_COL,GL_ARRAY_BUFFER,99999999, 1);
	buffer_set_data(ref->VBO_LINE_COL, GL_ARRAY_BUFFER,   
		sizeof(vec4), 
		ref->colors_line, 
		GL_DYNAMIC_DRAW);

	buffer_describe(mat4)(ref->VBO_LINE_TRANS,GL_ARRAY_BUFFER,99999999, 10);
	buffer_set_data(ref->VBO_LINE_TRANS, GL_ARRAY_BUFFER,   
		sizeof(mat4), 
		ref->transforms_line, 
		GL_STATIC_DRAW);

		
	free(points_circle);
	free(points_sqare);
}
void model_free(Model * model[static 1]){
	assert(*model);
 
	free((*model)->transforms_circle);
	free((*model)->transforms_rect);

	free((*model)->colors_circle);
	free((*model)->colors_rect);
	free((*model)->colors_line);

	free((*model)->circles);
	free((*model)->rects);
	free((*model)->lines);
	free(*model);
}
 
#endif