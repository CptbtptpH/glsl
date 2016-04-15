// shader_test1.cpp : 定义控制台应用程序的入口点。
//

// glfw_glew_test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "math/Matrix.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "vgl.h"
#include "LoadShaders.h"
GLFWwindow* window;



enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 ,
				   vColor =1};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices =3;
void InitShader()
{


	GLfloat vertices[] = {
		// Positions         // Colors
		0.5f, -0.5f,0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,0.0f, 1.0f, 0.0f,  // Bottom Left
		0.0f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f   // Top 
	};

	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "../shaders/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "../shaders/triangles.frag" },
		{ GL_NONE, NULL },
	};
	GLuint program = Program::Load(shaders);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vPosition);
	
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 6* sizeof(GLfloat), (GLvoid*)( 3* sizeof(GLfloat)));
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0); // Unbind VAO
}

void initScene(int w, int h)
{

	glViewport(0, 0, (GLint)w, (GLint)h);

	InitShader();

}

void  drawScene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glBindVertexArray(0);
}
void resizeGL(GLFWwindow*, int w, int h)
{

	// 重置当前的视口  
	glViewport(0, 0, (GLint)w, (GLint)h);

	//InitShader();

	return;

}
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int _tmain(int argc, _TCHAR* argv[])
{


	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	const unsigned char * ver = glGetString(GL_VERSION);

	if (GLEW_OK != err)
	{
		printf("init GLEW error!\n");

		return 0;
	}
	else
	{
		printf("init GLEW ok!\n");
		printf("openglVersion-%s.\r\n", ver);

	}
	initScene(800, 600);
	glfwSetWindowSizeCallback(window, resizeGL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 传递消息鼠标键盘等




		drawScene();

 
		glfwSwapBuffers(window);

	}

	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}