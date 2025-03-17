#pragma once


//#include "cglm/types.h"
#include "math/math.h"
#include "math/types.h"


typedef struct{
	float screen_w,screen_h;
 

	
	unsigned _BitInt(3) states;

	unsigned int UBO_PV;

	Mat4 m4_projection;
	Mat4 m4_view;
	Vec3 v3_camp_pos;
	//Mat4 m4_pv;



}Camera;




void cam_move_to(Camera cam[static 1],Vec3);
void cam_move_by(Camera cam[static 1],Vec3);
 
void cam_commit_buffers(Camera cam[static 1]);
void cam_screen_size(Camera cam[static 1],int w,int h);
void cam_init(Camera *cam[static 1]);
void cam_free(Camera *cam[static 1]);

 

#ifdef CAMERA_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
 
 
 

void  cam_init(Camera *cam[static 1]){
	cam_free(cam);
	
	 
 
	 *cam=calloc(1, sizeof(Camera));
	Camera *ref =  *cam;
 
	//binding point 0
	glGenBuffers(1, &(ref)->UBO_PV);
	glBindBuffer(GL_UNIFORM_BUFFER, (ref)->UBO_PV);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4), NULL, GL_STATIC_DRAW);  
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	 
	 
}
 
 
void cam_free(Camera *cam[static 1]){
	if(*cam != nullptr){
 
		free(*cam);
		printf("free cam\n");
	
	}
}
void cam_commit_buffers(Camera cam[static 1]){


	Mat4 pv = math_new_trans(cam->v3_camp_pos);

	pv = math_mul_m4_m4(cam->m4_projection, pv);
	 
 


	glBindBuffer(GL_UNIFORM_BUFFER, cam->UBO_PV);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), &pv);  
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void cam_screen_size(Camera cam[static 1],int w,int h){
	int z = max(w,h) * 2;
 
	// float ww = w/cam->coord_div;
	// float hh =  h/cam->coord_div;
 
	cam->screen_h =  h;
	cam->screen_w =  w;

	// float half_w = ww/2.;
	// float half_h = hh/2.;
	
	//printf("orto %f %f\n",hh,ww );
	cam->m4_projection = math_ortho(0, w, 0, h, 0, z);
	cam->m4_view = math_new_trans((Vec3){-w/2.,-h/2.,0});
	// cam->m4_view = math_new_trans(cam->v3_camp_pos);
	 
	 
	// cam->m4_pv = math_mul_m4_m4(cam->m4_projection, cam->m4_view );
 
 
}
void cam_move_by(Camera cam[static 1],Vec3 pos){

	cam->v3_camp_pos.x += pos.x;
	cam->v3_camp_pos.y += pos.y;
	cam->v3_camp_pos.z += pos.z;

	Vec3 cam_pos = {
		.x = cam->v3_camp_pos.x - cam->screen_w / 2.,
		.y = cam->v3_camp_pos.y - cam->screen_h / 2.,
		.z =cam->v3_camp_pos.z,
		
	};
	cam->m4_view = math_new_trans(cam_pos);

}
void cam_move_to(Camera cam[static 1],Vec3 pos){
 
 
	cam->v3_camp_pos = pos;
 
	Vec3 cam_pos = {
		.x = pos.x - cam->screen_w / 2.,
		.y = pos.y - cam->screen_h / 2.,
		.z = pos.z,
		
	};
	

 
}
 
#endif