// shader_test1.cpp : 定义控制台应用程序的入口点。
//

// glfw_glew_test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "public/dataType.h"

//#include "math/MyMath.h"
//#include "math/Vector.h"

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
	vTexture = 2,
	vNormal = 3
};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 36;
GLuint g_program = 0;
GLuint g_programLight = 0;

ActiveEngine::aeMat4f viewMat;
ActiveEngine::aeMat4f modelMat;
ActiveEngine::aeMat4f projectionMat;

ActiveEngine::aeMat4f MVPmat;

GLuint gl_texID;
GLuint gl_texID1;
//GLuint LoadTexture(char* imgName, int & width, int & height)
//{
//	if (nullptr == imgName)
//		return -1;
//
//	FREE_IMAGE_FORMAT  fif = FIF_UNKNOWN;
//	FIBITMAP *dib = nullptr;
//
//	fif = FreeImage_GetFileType(imgName, 0);
//	if (fif == FIF_UNKNOWN)
//		fif = FreeImage_GetFIFFromFilename(imgName);
//	if (fif == FIF_UNKNOWN)
//		printf("图片格式不支持\n");
//
//	if (FreeImage_FIFSupportsReading(fif))
//		dib = FreeImage_Load(fif, imgName);
//	BYTE *bits = FreeImage_GetBits(dib);
//	//get the image width and height
//	width = FreeImage_GetWidth(dib);
//	height = FreeImage_GetHeight(dib);
//	//if this somehow one of these failed (they shouldn't), return failure
//	if ((bits == 0) || (width == 0) || (height == 0))
//		printf("图片加载错误\n");
//
//	GLuint texID = 0;
//	glGenTextures(1, &texID);
//	glBindTexture(GL_TEXTURE_2D, texID);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// Set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
//	glGenerateMipmap(GL_TEXTURE_2D);
//	if (dib != nullptr) // 释放资源
//		FreeImage_Unload(dib);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	return texID;
//}
void InitShader()
{
	//texture 

	int width = 0;
	int height = 0;
	char * imgName = "box.jpg";
	//gl_texID = LoadTexture(imgName, width, height);
	imgName = "dog.jpg";
	//gl_texID1 = LoadTexture(imgName, width, height);
	

	//GLfloat vertices[] = {
	//	// Positions         // Colors      // textue
	//	-1.0f, -1.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f,  // Bottom Right
	//	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Left
	//	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,// Top 
	//	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	//};
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "../shaders/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "../shaders/triangles.frag" },
		{ GL_NONE, NULL },
	};
	 g_program = Program::Load(shaders);


	 shaders[0] =
	 { GL_VERTEX_SHADER, "../shaders/lamp.vert" };
	 shaders[1] =
	 { GL_FRAGMENT_SHADER, "../shaders/lamp.frag" };

	 shaders[2] = { GL_NONE, NULL };
	
	 g_programLight = Program::Load(shaders);


	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	 glUseProgram(g_program);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(vNormal);
	glBindVertexArray(0); // Unbind VAO


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}
aeVec3f viewPos = aeVec3f({4.0f, 4.0f, 4.0f });
void initScene(int w, int h)
{

	glViewport(0, 0, (GLint)w, (GLint)h);
 
	projectionMat.Perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
 
	viewMat.LookAt(viewPos, aeVec3f({ 0.0f, 0.0f, 0.0f }), aeVec3f({ 0.0f, 1.0f, 0.0f }));

	modelMat = aeMat4f();
	
	//MVPmat = projectionMat *viewMat *modelMat;
	 
	InitShader();

}
aeVec3f lightPos = aeVec3f({0.0f, 0.8f, 0.0f });

void  drawScene()
{
	glUseProgram(g_program);
	GLint modelLoc = glGetUniformLocation(g_program, "model");
	GLint viewLoc = glGetUniformLocation(g_program, "view");
	GLint projLoc = glGetUniformLocation(g_program, "projection");

	GLint objectColorLoc = glGetUniformLocation(g_program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(g_program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(g_program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(g_program, "viewPos");
	//lightPos.X= sin(glfwGetTime()) * 1.0f;
	//lightPos.Y = cos(glfwGetTime() )*1.0f;
	lightPos.Y = 1.5 + sin(glfwGetTime()*0.5f);
	glUniform3f(viewPosLoc, viewPos.X, viewPos.Y, viewPos.Z);
	glUniform3f(lightPosLoc, lightPos.X, lightPos.Y, lightPos.Z);

	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); //
	//modelMat.Rotate((GLfloat)glfwGetTime() * 0.01f, 1.0f, 1.0f, 0.0f);

 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE,modelMat.get() );
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.get());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMat.get());
 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glBindVertexArray(0);

	glUseProgram(g_programLight);

	modelLoc = glGetUniformLocation(g_programLight, "model");
	viewLoc = glGetUniformLocation(g_programLight, "view");
	projLoc = glGetUniformLocation(g_programLight, "projection");

	//modelMat.Rotate((GLfloat)glfwGetTime() * 0.01f, 1.0f, 1.0f, 0.0f);
 
	aeMat4f lightModelMat = aeMat4f();
	lightModelMat.Translate(lightPos.X, lightPos.Y, lightPos.Z);
	lightModelMat.Scale(0.1f, 0.1f, 0.1f);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, lightModelMat.get());
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.get());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMat.get());
	 

	glBindVertexArray(VAOs[Triangles]);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glBindVertexArray(0);

	modelMat.Identity();
}
void resizeGL(GLFWwindow*, int w, int h)
{
	// 重置当前的视口  
	glViewport(0, 0, (GLint)w, (GLint)h);
  
	projectionMat.Perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
 
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