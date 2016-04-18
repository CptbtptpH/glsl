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
enum Attrib_IDs { vPosition = 0 ,
				   vColor =1};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices =3;
GLuint g_program = 0;

ActiveEngine::aeMat4f viewMat4;
ActiveEngine::aeMat4f model;
ActiveEngine::aeMat4f projection;
ActiveEngine::aeMat4f MVPmat;
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
	 g_program = Program::Load(shaders);
	 glUseProgram(g_program);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(vPosition);
	
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 6* sizeof(GLfloat), (GLvoid*)( 3* sizeof(GLfloat)));
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0); // Unbind VAO
}

void initScene(int w, int h)
{

	glViewport(0, 0, (GLint)w, (GLint)h);
	ActiveEngine::aeMat4f projMat;
	projMat.Perspective(45.0f, (GLfloat)w / (GLfloat)h, .1, 100.0);
	projection = projMat;

	ActiveEngine::aeMat4f viewMat;
	// 相机位置
	//viewMat.LookAt(aeVec3f({ 0.0f,-0.0f, -2.0f }), aeVec3f({0.0f, 0.0f, 0.0f }), aeVec3f({ 0.0f, 1.0f, 0.0f }));
	//viewMat.Translate(0, 0, -3);
	aeVec3f eye = aeVec3f({ 0.0f, -0.0f, -0.0f });
	aeVec3f center = aeVec3f({ -1.0f, -0.0f, -0.0f });
	aeVec3f up = aeVec3f({ 0.0f, 1.0f, 0.0f });
	
	aeVec3f d = center - eye;
	d.Normalize();

	aeVec3f r = Cross(up, d);
	r.Normalize();

	aeVec3f u = Cross(d, r);
	u.Normalize();

	aeFLOAT x = Dot(eye, r);
	aeFLOAT y = Dot(eye, u);
	aeFLOAT z = Dot(eye, d);


	aeMat4f tranMat;
	tranMat.Identity();
	tranMat.C[3].X = x;// Dot(r, eye);
	tranMat.C[3].Y = y;// Dot(u, eye);
	tranMat.C[3].Z = z;// Dot(d, eye);

	aeMat4f rotMat;
	rotMat.Identity();
	rotMat.C[0].X = r.X;
	rotMat.C[0].Y = u.X;
	rotMat.C[0].Z = d.X;

	rotMat.C[1].X = r.Y;
	rotMat.C[1].Y = u.Y;
	rotMat.C[1].Z = d.Y;

	rotMat.C[2].X = r.Z;
	rotMat.C[2].Y = u.Z;
	rotMat.C[2].Z = d.Z;

	aeMat4f result = tranMat * rotMat;
	 
	viewMat = result;

	model = aeMat4f();

	MVPmat.Identity();
	MVPmat = projection *viewMat *model;

	InitShader();

}
void  drawScene()
{

	GLint modelLoc = glGetUniformLocation(g_program, "model");
	GLint viewLoc = glGetUniformLocation(g_program, "view");
	GLint projLoc = glGetUniformLocation(g_program, "projection");
	GLint MVP = glGetUniformLocation(g_program, "MVP");
 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE,model.get() );
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMat4.get());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.get());
	glUniformMatrix4fv(MVP, 1, GL_FALSE, MVPmat.get());
	 
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