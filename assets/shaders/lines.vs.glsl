#version 430 core

layout (location = 0) in vec3 vpos;
 
 // +11,12,13
 
out vec4 frag_color;
 
uniform vec4 vcol;
uniform mat4 model;

layout (std140, binding = 0) uniform Matrices
{
    mat4 pv;

};


 

 

void main()
{
 
   	gl_Position = pv * model * vec4(vpos,1.0) ;
	
    frag_color = vcol;
	 
	
}