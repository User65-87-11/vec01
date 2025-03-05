#pragma warning(disable: 4996)

 
 

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdlib.h>
#include "cglm/cglm.h"
#include "buffer.h"

#include "test.h"
#include "camera.h"
#include "shader.h"

#include "model_square.h"
#include "model.h"


static void error_callback(int error, const char* description);
static void mouse_callback(GLFWwindow * w, double xpos, double ypos);
static void framebuffer_size_callback(GLFWwindow * w,int width, int height);
static void key_callback(GLFWwindow * w, int key, int scancode, int action, int mods);
static void debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam);   


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


GLuint shader;
Camera * camera;
Model_square * model_square;

Model * model;

int main(){

	print_me();


	
	//--------
    glfwSetErrorCallback(error_callback); 

    if (!glfwInit())
		exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple example", NULL, NULL);
	
    if (!window)
    {
		
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	
	glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback( window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
       
        return -1;
    }

	glDebugMessageCallback(debug_callback,nullptr);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	glfwSwapInterval(1);

	
	 
	shader = shader_compile(
		"../assets/shaders/simple.vs.glsl", 
		"../assets/shaders/simple.fs.glsl"
	);
	 
	cam_init(&camera);
	cam_screen_size(camera, SCR_WIDTH, SCR_HEIGHT);
	cam_move_to(camera, 0, 0);

	shader_use(shader);
	shader_bind_ubo("Matrices", camera->UBO_PV);
	
	//------------

	model_square_init(&model_square, 1);
  

	model_init(&model);
	model_circle_add(model,0,0,1);
	model_rect_add(model,4,4,1);

	model_rect_add(model,-2,-2,2);

 
	//---------
	 
	float last_time =glfwGetTime();
	int fps_cnt =0;
	char buffer [50];

	framebuffer_size_callback(window,SCR_WIDTH,SCR_HEIGHT);
	
	glPointSize(2);


    while (!glfwWindowShouldClose(window)){
		float current_time = glfwGetTime();
		fps_cnt+=1;
 

		if( current_time - last_time > 1.0f){

			last_time = current_time;
			snprintf(buffer, 50,"fps: %i \n", fps_cnt );
			glfwSetWindowTitle(window, buffer);
			
			fps_cnt = 0;
		}





		//--- LOGIC_START

		model_square_update(model_square);
		model_update(model);

		

		//--- LOGIC_END


		//_______DRAW_MODEL_SQUARE
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw your objects here

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		shader_use(shader);

		glBindVertexArray(model_square->VAO);

	//	glDrawArrays(GL_TRIANGLES,0,6);


		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restore fill mode
		//_________END_DRAW_MODEL_SQUARE

		//----DRAW_MODEL_LINE
		glBindVertexArray(model->VAO_CIRCLE);
		glDrawArraysInstanced(GL_LINE_LOOP,0,model->point_num_circ,model->num_inst_circle);

		glBindVertexArray(model->VAO_RECT);
		glDrawArraysInstanced(GL_LINE_LOOP,0,model->point_num_rect,model->num_inst_rect);


		glBindVertexArray(model->VAO_LINE);
		glDrawArraysInstanced(GL_LINE,0,model->point_num_line,model->num_inst_line);

		// draw lines and disable matrix

		//----END_DRAW_MODEL_LINE


		glfwSwapBuffers(window);
		glfwPollEvents();
	
    }
	
    glfwDestroyWindow(window);
 
    glfwTerminate();
	printf("exit ok");
	
	model_square_free(&model_square);
	model_free(&model);
	cam_free(&camera);
    exit(EXIT_SUCCESS);


	return 0;
};




static void error_callback(int error, const char* description)
{
   
    printf("Error %i, %s\n",error,description);
}

 
static void mouse_callback(GLFWwindow * window, double xpos, double ypos){

	camera->mouse_x = xpos / camera->coord_div - camera->screen_w/2.;
	camera->mouse_y = (camera->screen_h -  ypos)/ camera->coord_div + camera->screen_h/2.;

	//printf("%f %f\n", camera->mouse_x, camera->mouse_y);
	
	Model_square_inst inst = model_square_inst(model_square, 0);
	model_square_move(&inst, camera->mouse_x, camera->mouse_y);
	
	 
}
static void framebuffer_size_callback(GLFWwindow * window, int w, int  h){
	
	glViewport(0, 0, w, h);
	cam_screen_size(camera, w, h);
}
static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods){
 
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} 

}

static void debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam){
		
	printf("Opengl Error:\n");
	printf("Id: %i \n",id);
	printf("Type: %x \n",type);	
	printf("MSG: %s \n",message);	
}