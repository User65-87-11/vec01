

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#define GLFW_INCLUDE_NONE	
#define ARRAY_IMPLEMENTATION	
#define WAVEFRONT_IMPLEMENTATION
#define BUFFER_IMPLEMENTATION	
#define CAMERA_IMPLEMENTATION	
#define LIST_IMPLEMENTATION	
#define MODEL_SQUARE_IMPLEMENTATION	
#define SHADER_IMPLEMENTATION	
#define ANIMATION_IMPLEMENTATION	
#define RECT_VEC_IMPLEMENTATION	
#define MODELS_IMPLEMENTATION	
#define MATH_IMPLEMENTATION	
#define GAME_IMPLEMENTATION	


#include "math/math.h"
#pragma warning(disable: 4996)

 
 
 
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdlib.h>
#include <time.h>
// #include "cglm/cglm.h"
// #include "buffer.h"
// #include "cglm/affine2d.h"

#include "list.h"
#include "camera.h"
#include "shader.h"

#include "game.h" 

#include "animation.h"
#include "animation_key.h"
 
#include "models/models_2d.h"
#include "math/types.h"
#include "mouse.h"

 


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


void mouse_button_callback (struct GLFWwindow *, int, int, int);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


GLuint shader;

Camera * camera;

Game_ns *game_ns;
//Model_square * model_square;

//Model * model;
Mod2d_ns * mod2d_ns;

Mouse  mouse;

bool do_anim=false;


#define PI 3.14159265358979323846

#define RAD (PI/180.0)


 
int main(){

	 
	 
	//printf("float %f\n",mod_rect_ns.val);
	
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
	glfwSetMouseButtonCallback(window, mouse_button_callback);
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
 
	cam_move_to(camera,(Vec3){SCR_WIDTH/2., SCR_HEIGHT/2. , 0.0f});
 
	shader_use(shader);
	shader_bind_ubo("Matrices", camera->UBO_PV);
	

	mod2d_ns_init(&mod2d_ns);


	game_init(&game_ns);
	//------------
 
 


	{
		for(int i=0;i< 50000;i++)
		{
			mod2d_ns->createRect((Vec3){0.0f,0.0f,0},50,50);
		}

		mod2d_ns->createRect((Vec3){0.0f,0.0f,0},50,50);

		mod2d_ns->createRect((Vec3){300.0f,100.0f,0},50,50);

		mod2d_ns->createRect((Vec3){200.0f,500.0f,0},50,50);
 

		mod2d_ns->createCirc((Vec3){400.0f,000.0f,0},200);

		mod2d_ns->createCirc((Vec3){60.0f,100.0f,0},40);
 

		Mod_head *h = mod2d_ns->createLine((Vec3){0.0f,1000.0f,0},(Vec3){0.0f,-1000.0f,0});

		mod2d_ns->change_color(h,(Vec4){255,0,0,100});
		 
		h = mod2d_ns->createLine((Vec3){-1000.0f,0.0f,0},(Vec3){1000.0f,0.0f,0});
		mod2d_ns->change_color(h,(Vec4){0,255,0,100});
	}
	
	 
 
	 
	//ValueAnimation * va;
	//anim_value_init(&va,rect2.m_rect->rot,anim_1);
	//anim_ease_fun_set(va, anim_easeInOutQuad);

 
	 

	//---------
	 
	float last_time =glfwGetTime();
	float fps_time =last_time;
	float current_time = last_time;

	float time_delta = 0;
	int fps_cnt =0;
	char buffer [50];

	framebuffer_size_callback(window,SCR_WIDTH,SCR_HEIGHT);
	
	glPointSize(2);
	glLineWidth(2);
 
  


 
    while (!glfwWindowShouldClose(window)){
		
		
		current_time = glfwGetTime();
		time_delta = current_time - last_time;
		last_time = current_time;

		fps_cnt+=1;
 
	
		if( current_time - fps_time > 1.0f){

			fps_time = current_time;
			snprintf(buffer, 50,"fps: %i \n", fps_cnt );
			glfwSetWindowTitle(window, buffer);
			
			fps_cnt = 0;
		}

 
 
		
	
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		game_ns->update_state();

		game_ns->render();

		
		
 
		glfwSwapBuffers(window);
		glfwPollEvents();
	
    }
	
    glfwDestroyWindow(window);
 
    glfwTerminate();
	printf("exit ok");
	
 
	mod2d_ns_free(&mod2d_ns);
	//model_free(&model);
	cam_free(&camera);
	game_free(&game_ns);
	//anim_value_free(&va);
    exit(EXIT_SUCCESS);


	return 0;
};




static void error_callback(int error, const char* description)
{
   
    printf("Error %i, %s\n",error,description);
}

 
static void mouse_callback(GLFWwindow * window, double xpos, double ypos){


	mouse.prev_x =  mouse.mouse_x;
	mouse.prev_y =  mouse.mouse_y;

	mouse.mouse_x = xpos ;
	mouse.mouse_y = camera->screen_h -   ypos ;
	
	 
	
}
void mouse_button_callback (struct GLFWwindow *, int btn, int action, int mods){
	/**
	[in]	window	The window that received the event.
	[in]	button	The mouse button that was pressed or released.
	[in]	action	One of GLFW_PRESS or GLFW_RELEASE. Future releases may add more actions.
	[in]	mods	Bit field describing which modifier keys were held down.

	#define GLFW_MOUSE_BUTTON_LEFT   GLFW_MOUSE_BUTTON_1
	#define GLFW_MOUSE_BUTTON_RIGHT   GLFW_MOUSE_BUTTON_2

	*/

//	printf("action: %i %f %f\n", action,camera->mouse_x,camera->mouse_y);
 

	mouse.mouse_mods = mods;
	mouse.mouse_action = action;
	mouse.mouse_button = btn;

	if(action == GLFW_PRESS)
	{
		mouse.press_x = mouse.mouse_x;
		mouse.press_y = mouse.mouse_y;
		mouse.press_time = glfwGetTimerValue();
		mouse.is_pressed = true;
		mouse.is_released = false;
	}else  
	{
		mouse.release_x = mouse.mouse_x;
		mouse.release_y = mouse.mouse_y;
		mouse.release_time = glfwGetTimerValue();
		mouse.is_released = true;
		mouse.is_pressed = false;
	}
 

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
	
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		printf("do anim?\n");
		do_anim = true;

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