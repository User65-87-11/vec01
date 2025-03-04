#pragma once


#include "cglm/types.h"

 
typedef struct{
	float screen_w,screen_h;
 
	float mouse_x,mouse_y;
	float coord_div;
	unsigned int UBO_PV;

	mat4* m4_projection;
	mat4* m4_view;
	vec3* v3_camp_pos;
	mat4* m4_pv;



}Camera;




void cam_move_to(Camera cam[static 1],float x,float y);
void cam_screen_size(Camera cam[static 1],int w,int h);
void cam_init(Camera *cam[static 1]);
void cam_free(Camera *cam[static 1]);
 




#ifdef CAMERA_IMPLEMENTATION
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include "cglm/affine-post.h"
#include "cglm/affine-pre.h"
#include "cglm/affine.h"
#include "cglm/io.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/util.h"
 

 

void  cam_init(Camera *cam[static 1]){
	cam_free(cam);
	
	 
	//non dynamic mem camera
	 *cam=calloc(1, sizeof(Camera));
	 
	Camera * ref = *cam;
 
	
	ref->m4_projection = calloc(1,sizeof(mat4) );
	ref->m4_view = calloc(1,sizeof(mat4) );

	ref->v3_camp_pos = calloc(1,sizeof(vec3));
	ref->m4_pv = calloc(1,sizeof(mat4));
	ref->coord_div = 50.0;
	//binding point 0
	glGenBuffers(1, &ref->UBO_PV);
	glBindBuffer(GL_UNIFORM_BUFFER, ref->UBO_PV);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4), NULL, GL_STATIC_DRAW);  
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	 
	 
}
 

void cam_free(Camera *cam[static 1]){
	if(*cam != nullptr){
		free((*cam)->m4_projection);
		free((*cam)->m4_view);
		free((*cam)->v3_camp_pos);
		free((*cam)->m4_pv);
		free(*cam);
		printf("free cam\n");
	
	}
}
 
void cam_screen_size(Camera cam[static 1],int w,int h){
	int z = max(w,h) * 2;
 
	int ww = w/cam->coord_div;
	int hh =  h/cam->coord_div;
 
	cam->screen_h =  hh;
	cam->screen_w =  ww;

	glm_ortho(-ww/2., ww/2., -hh/2., hh/2., -z, z, (vec4 *)cam->m4_projection);
	//glm_mat4_identity((vec4 *)cam->m4_projection);

//	vec4 center ={w>>1,h>>1,0.f,1.f};
	glm_mat4_identity((vec4 *)cam->m4_view);
 
	// glm_translate((vec4 *)cam->m4_view,center);
	// glm_rotate_x((vec4 *)cam->m4_view, glm_rad(-60.0f), (vec4 *)cam->m4_view);
	// glm_rotate_z((vec4 *)cam->m4_view, glm_rad(45.0f), (vec4 *)cam->m4_view);
	glm_translate((vec4 *)cam->m4_view, (float*)cam->v3_camp_pos);
	glm_mat4_mul((vec4 *)cam->m4_projection, (vec4 *)cam->m4_view, (vec4 *)cam->m4_pv);

	 
	glBindBuffer(GL_UNIFORM_BUFFER, cam->UBO_PV);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), cam->m4_pv);  
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	


	

	//cam_move_to_tile(cam,0,0);
	printf("cam_screen_size \n");
 
}

void cam_move_to(Camera cam[static 1],float x,float y){
 
	float * pos =(float *)cam->v3_camp_pos;
	pos[0] = x;
	pos[1] = y;
	pos[2] = 0;

	glm_mat4_identity((vec4 *)cam->m4_view);
	glm_translate((vec4 *)cam->m4_view, (float*)cam->v3_camp_pos);
	glm_mat4_mul((vec4 *)cam->m4_projection, (vec4 *)cam->m4_view, (vec4 *)cam->m4_pv);


	glBindBuffer(GL_UNIFORM_BUFFER, cam->UBO_PV);
	glBufferSubData(GL_UNIFORM_BUFFER, 0  , sizeof(vec4), cam->m4_pv);  
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	printf("cam pos: %f %f \n", pos[0], pos[1]);
 
}
#endif