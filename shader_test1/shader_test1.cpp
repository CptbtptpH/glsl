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

const GLuint NumVertices = 6;
GLuint g_program = 0;

ActiveEngine::aeMat4f viewMat;
ActiveEngine::aeMat4f modelMat;
ActiveEngine::aeMat4f projectionMat;

ActiveEngine::aeMat4f MVPmat;
GLuint framebuffer;
GLuint gl_texID;
GLuint gl_texID1;
GLuint textureColorbuffer;

GLuint gWindowWidth = 0;
GLuint gWindowHeight = 0;

GLuint LoadTexture(char* imgName, int & width, int & height)
{
	if (nullptr == imgName)
		return -1;

	FREE_IMAGE_FORMAT  fif = FIF_UNKNOWN;
	FIBITMAP *dib = nullptr;

	fif = FreeImage_GetFileType(imgName, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(imgName);
	if (fif == FIF_UNKNOWN)
		printf("图片格式不支持\n");

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, imgName);
	BYTE *bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		printf("图片加载错误\n");

	GLuint texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
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

	return texID;
}
// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLuint screenWidth,GLuint screenHeight)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screenWidth, screenHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void InitShader()
{
	//texture 

	int width = 0;
	int height = 0;
	char * imgName = "box.jpg";
	gl_texID = LoadTexture(imgName, width, height);
	imgName = "dog.jpg";
	gl_texID1 = LoadTexture(imgName, width, height);
	

	//GLfloat vertices[] = {
	//	// Positions         // Colors      // textue
	//	-1.0f, -1.0f,0.0f, 1.0f, 0.0f, 0.0f, 1.0f,1.0f,  // Bottom Right
	//	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Left
	//	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,// Top 
	//	1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	//};
	GLfloat vertices[] = {
		-100.5f, -100.5f, -0.5f, 0.0f, 0.0f,
		100.5f, -100.5f, -0.5f, 1.0f, 0.0f,
		100.5f, 100.5f, -0.5f, 1.0f, 1.0f,
		100.5f, 100.5f, -0.5f, 1.0f, 1.0f,
		-100.5f, 100.5f, -0.5f, 0.0f, 1.0f,
		-100.5f, -100.5f, -0.5f, 0.0f, 0.0f
	};
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "../shaders/pulse.vert" },
		{ GL_FRAGMENT_SHADER, "../shaders/pulse.frag" },
		{ GL_NONE, NULL },
	};
	 g_program = Program::Load(shaders);
	 glUseProgram(g_program);

	 GLint winWidthLoc = glGetUniformLocation(g_program, "windowWidth");
	 glUniform1i(winWidthLoc, gWindowWidth);

	 GLint winHeightLoc = glGetUniformLocation(g_program, "windowHeight");
	 glUniform1i(winHeightLoc, gWindowHeight);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vPosition);
	
//	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (GLvoid*)( 3* sizeof(GLfloat)));
//	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(vTexture);
	glBindVertexArray(0); // Unbind VAO


	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// 
	textureColorbuffer = generateAttachmentTexture(false, false, 400, 400);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 400, 400); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

void initScene(int w, int h)
{

	glViewport(0, 0, (GLint)w, (GLint)h);
 
	projectionMat.Perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
 
	viewMat.LookAt(aeVec3f({ 0.0f,2.0f, 4.0f }), aeVec3f({0.0f, 0.0f, 0.0f }), aeVec3f({ 0.0f, 1.0f, 0.0f }));

	modelMat = aeMat4f();
	
	//MVPmat = projectionMat *viewMat *modelMat;
	 
	InitShader();
	
}
aeVec3f cubePositions[] = {

	aeVec3f({ 0.0f, 0.0f, 0.0f }),
	aeVec3f({ 2.0f, 5.0f, -15.0f }),
	aeVec3f({-1.5f, -2.2f, -2.5f }),
	aeVec3f({ -3.8f, -2.0f, -12.3f }),

	aeVec3f({ 2.4f, -0.4f, -3.5f }),
	aeVec3f({ -1.7f, 3.0f, -7.5f }),

	aeVec3f({ 1.3f, -2.0f, -2.5f }),
	aeVec3f({ 1.5f, 2.0f, -2.5f }),

	aeVec3f({ 1.5f, 0.2f, -1.5f }),
	aeVec3f({ -1.3f, 1.0f, -1.5 })
};
void  drawScene()
{

	GLint modelLoc = glGetUniformLocation(g_program, "model");
	GLint viewLoc = glGetUniformLocation(g_program, "view");
	GLint projLoc = glGetUniformLocation(g_program, "projection");

	//modelMat.Rotate((GLfloat)glfwGetTime() * 0.01f, 1.0f, 1.0f, 0.0f);

 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE,modelMat.get() );
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.get());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projectionMat.get());
	
	 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glUniform1i(glGetUniformLocation(g_program, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl_texID1);
	glUniform1i(glGetUniformLocation(g_program, "ourTexture2"), 1);
	
	glBindVertexArray(VAOs[Triangles]);
 
	GLfloat radius = 20.0f;
	GLfloat camX = sin(glfwGetTime()) * radius;
	GLfloat camZ = cos(glfwGetTime()) * radius;


	GLint dTimerlLoc = glGetUniformLocation(g_program, "dTimer");
	glUniform1f(dTimerlLoc, glfwGetTime());

	//modelMat.Rotate((GLfloat)glfwGetTime() * 0.01f, 1.0f, 1.0f, 0.0f);


	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMat.get());

	viewMat.LookAt(aeVec3f({ camX, 0.0f, camZ }), aeVec3f({ 0.0f, 0.0f, 0.0f }), aeVec3f({ 0.0f, 1.0f, 0.0f }));
	
 
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat.get());
	//for (GLuint i = 0; i < 10; i++)
	//{
	//	modelMat.Identity();
	//	modelMat.Translate(cubePositions[i].X, cubePositions[i].Y, cubePositions[i].Z);
	//	GLfloat angle = 20.0f * i;
	//	modelMat.Rotate(angle,  1.0f, 0.3f, 0.5f );
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMat.get());

	//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//}

	glDrawArrays(GL_TRIANGLES,0,6);

	//glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glBindVertexArray(0);
}
void resizeGL(GLFWwindow*, int w, int h)
{
	gWindowWidth = w;
	gWindowHeight = h;
	// 重置当前的视口  
	glViewport(0, 0, (GLint)w, (GLint)h);
  
	projectionMat.Perspective(75.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);


	glUseProgram(g_program);
	GLint winWidthLoc = glGetUniformLocation(g_program, "windowWidth");
	glUniform1i(winWidthLoc, gWindowWidth);

	GLint winHeightLoc = glGetUniformLocation(g_program, "windowHeight");
	glUniform1i(winHeightLoc, gWindowHeight);

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
	gWindowWidth = WindowWidth;
	gWindowHeight = WindowHeight;
	initScene(gWindowWidth, gWindowHeight);

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