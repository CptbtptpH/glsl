// shader_test1.cpp : 定义控制台应用程序的入口点。
//

// glfw_glew_test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "public/dataType.h"

//#include "math/MyMath.h"
#include "math/Vector.h"

#include "math/Matrix.h"

using namespace ActiveEngine;

#include <iostream>
#include <iomanip>
#include <string>

#include "vgl.h"
#include "LoadShaders.h"

GLFWwindow* window;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs
{ 
	vPosition = 0 ,
	vColor =1,
	vTexture = 2
};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices =4;
GLuint g_program = 0;

ActiveEngine::aeMat4f viewMat;
ActiveEngine::aeMat4f modelMat;
ActiveEngine::aeMat4f projectionMat;

ActiveEngine::aeMat4f MVPmat;

GLuint gl_texID;

void InitShader()
{
	//texture 
	FREE_IMAGE_FORMAT  fif = FIF_UNKNOWN;
	FIBITMAP *dib  = nullptr;
	BYTE* bits = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;
	
	char * imageName = "box.jpg";
	fif = FreeImage_GetFileType(imageName, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(imageName);
	if (fif == FIF_UNKNOWN)
		printf("图片格式不支持\n");

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, imageName);
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		printf("图片加载错误\n");

	
	glGenTextures(1, &gl_texID);
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (dib != nullptr) // 释放资源
		FreeImage_Unload(dib);

	glBindTexture(GL_TEXTURE_2D, 0);


	GLfloat vertices[] = {
		// Positions         // Colors      // textue
		-0.5f, -0.5f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f,  // Bottom Right
		-0.5f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Left
		0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,// Top 
		0.5f, -0.5f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
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
	 g_program = Program::Load(shaders);
	 glUseProgram(g_program);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vPosition);
	
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (GLvoid*)( 3* sizeof(GLfloat)));
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(vTexture);
	glBindVertexArray(0); // Unbind VAO
}

void initScene(int w, int h)
{

	glViewport(0, 0, (GLint)w, (GLint)h);
 
	projectionMat.Perspective(75.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
 
	viewMat.LookAt(aeVec3f({ 0.0f,0.0f, 2.0f }), aeVec3f({0.0f, 0.0f, 0.0f }), aeVec3f({ 0.0f, 1.0f, 0.0f }));

	modelMat = aeMat4f();

	//MVPmat = projectionMat *viewMat *modelMat;
	 
	InitShader();

}
void  drawScene()
{

	GLint modelLoc = glGetUniformLocation(g_program, "model");
	GLint viewLoc = glGetUniformLocation(g_program, "view");
	GLint projLoc = glGetUniformLocation(g_program, "projection");
	
 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE,modelMat.get() );
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.get());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMat.get());
	
	 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_QUADS, 0, NumVertices);
	glBindVertexArray(0);
}
void resizeGL(GLFWwindow*, int w, int h)
{
	// 重置当前的视口  
	glViewport(0, 0, (GLint)w, (GLint)h);
  
	projectionMat.Perspective(75.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
 
	return;

}
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const int WindowWidth = 800;
const int WindowHeight = 600;
int _tmain(int argc, _TCHAR* argv[])
{
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WindowWidth, WindowHeight, "Hello World", NULL, NULL);
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

	printf("openglVersion-%s.\r\n", ver);
	if (GLEW_OK != err)
	{
		printf("init GLEW error!\n");
		return 0;
	}
	else
	{
		printf("init GLEW ok!\n");
	}
	initScene(WindowWidth, WindowHeight);

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
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (GLFW_KEY_A== key && action == GLFW_PRESS)
	{

	}
	else if (GLFW_KEY_D == key && action == GLFW_PRESS)
	{

	}
}