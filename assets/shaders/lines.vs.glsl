#version 430 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec4 vcol;
 

layout (location = 10) in mat4 model;
 // +11,12,13
 
out vec4 frag_color;
 
layout (std140, binding = 0) uniform Matrices
{
    mat4 pv;

};


 

 

void main()
{
 
   	gl_Position = pv * model * vec4(vpos,1.0) ;
	
    frag_color = vcol;
	 
	
}