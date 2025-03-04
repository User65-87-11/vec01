#pragma once


typedef struct WF WF;

 
void wf_load(WF ** wf, const char* fname);
void wf_free(WF ** wf);

void wf_get_vert_data(WF * wf ,float ** data_out, size_t *len_out);
void wf_get_norm_data(WF * wf,float ** data_out, size_t* len_out );
void wf_get_textcoord_data(WF * wf ,float ** data_out, size_t *len_out);

#ifdef WF_IMPLEMENTATION



#include <assert.h> 
#include <stdio.h>
#include <stdlib.h>
#include "tinyobj/tinyobj_loader_c.h"

typedef struct WF{
	tinyobj_attrib_t  attrib;
	tinyobj_shape_t *shapes; //heap
	size_t num_shapes;
	tinyobj_material_t *materials;//heap 
	size_t num_materials;//
} WF;
 

void wf_file_reader_handler(void *ctx, const char *filename, int is_mtl, const char *obj_filename, char **buf, size_t *len){
	FILE * f ;
	int err =  fopen_s(&f, filename, "rb");
	printf("file error: %i\n",errno);
	assert(err == 0);
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	*buf = malloc(size);
	assert(*buf !=nullptr);
	size_t sz = fread_s(*buf, size, sizeof(char), size, f);
	*len = sz;
	fclose(f);

}


void wf_load(WF ** wf, const char* fname){

	printf("***wf_load\n");
	*wf = calloc(1,sizeof(WF));
	WF * ref = *wf;
	tinyobj_parse_obj(
		&ref->attrib,
		&ref->shapes,
		&ref->num_shapes,
		&ref->materials,
		&ref->num_materials,
		fname,
		wf_file_reader_handler,
		nullptr,
		TINYOBJ_FLAG_TRIANGULATE 
	);
	 
	// printf("num_vertices: %i \n",ref->attrib.num_vertices);
	// printf("num_faces: %i \n",ref->attrib.num_faces);
	// printf("face_num_verts: %i \n",ref->attrib.face_num_verts);
	// printf("num_face_num_verts: %i \n",ref->attrib.num_face_num_verts);
	// printf("num_shapes: %i \n",ref->num_shapes);
	// printf("shapes->length: %i \n",ref->shapes->length);
	//obj_get_vert_v(const obj *, int, float *)

	// wf_get_vertices(ref);
	// wf_get_normals(ref);
	// wf_get_textcoords(ref);


	
}
void wf_get_vert_data(WF * wf ,float * data_out[static 1], size_t *size_out){
 
	printf("wf_get_vert_data\n");
 
	
 
	*data_out = malloc(sizeof(float[3])*wf->attrib.num_faces);
	assert(*data_out);
	float * ref = *data_out;
	*size_out = sizeof(float[3])*wf->attrib.num_faces;

 
	int data_idx =0;
	for(int i=0;i<wf->attrib.num_faces;i++)
	{
 
		//for(int f=0;f<3;f++)
		{

			tinyobj_vertex_index_t f_att = wf->attrib.faces[i];
		
			float * vert= &wf->attrib.vertices[f_att.v_idx*3];
			
			ref[data_idx++] = vert[0];
			ref[data_idx++] = vert[1];
			ref[data_idx++] = vert[2];
 
			// printf("%i \n",f_att.v_idx);
			// printf("%f %f %f\n",vert[0],vert[1],vert[2]);
		}
	
	}
	 
}
void wf_get_norm_data(WF * wf,float ** data_out, size_t* len_out ){
 
	printf("wf_get_norm_data\n");
	*data_out = malloc(sizeof(float[3])*wf->attrib.num_faces);
	assert(*data_out);
	*len_out = sizeof(float[3])*wf->attrib.num_faces;
	float * ref = *data_out;

 
	int data_idx =0;
	for(int i=0;i<wf->attrib.num_faces;i++)
	{
 
		//for(int f=0;f<3;f++)
		{

			tinyobj_vertex_index_t f_att = wf->attrib.faces[i];
		
			float * vert= &wf->attrib.normals[f_att.vn_idx*3];
			
			ref[data_idx++] = vert[0];
			ref[data_idx++] = vert[1];
			ref[data_idx++] = vert[2];
			
			 printf("%i \n",f_att.v_idx);
			 printf("%f %f %f\n",vert[0],vert[1],vert[2]);
		}
	
	}
  
}
void wf_get_textcoord_data(WF * wf ,float ** data_out, size_t *len_out){

	printf("wf_get_textcoord_data\n");
	*data_out = malloc(sizeof(float[2])*wf->attrib.num_faces);
	assert(*data_out);
	*len_out = sizeof(float[2])*wf->attrib.num_faces;

	float * ref = *data_out;
 
	int data_idx =0;
	for(int i=0;i<wf->attrib.num_face_num_verts;i++)
	{
 
		for(int f=0;f<3;f++)
		{

			tinyobj_vertex_index_t f_att = wf->attrib.faces[i*3 + f];
		
			float * vert= &wf->attrib.texcoords[f_att.vt_idx*2];
			
			ref[data_idx++] = vert[0];
			ref[data_idx++] = vert[1];
	 
			
			// printf("%i \n",f_att.v_idx);
			// printf("%f %f %f\n",vert[0],vert[1],vert[2]);
		}
	
	}


}
void wf_free(WF ** wf){
	if(*wf){
 
		free(*wf);
	}
	*wf = nullptr;
}

#endif