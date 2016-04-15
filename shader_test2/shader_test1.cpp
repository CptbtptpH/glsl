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

void initScene(int w, int h)
{

	// 重置当前的视口  
	glViewport(0, 0, (GLint)w, (GLint)h);

	// 选择投影矩阵  
	glMatrixMode(GL_PROJECTION);
	// 重置投影矩阵  
	glLoadIdentity();

	// 设置视口的大小  
	//gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	ActiveEngine::aeMat4f ad;
	ad.Perspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	ActiveEngine::aeMat4f regid;
	regid.Translate(-1, 0, -5); // 相机位置

	ad = ad*regid;

	float *mm = ad.get(); // 矩阵构成
	glMultMatrixf(mm);

	// 选择模型观察矩阵  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 启用阴影平滑  
	glShadeModel(GL_SMOOTH);
	// 黑色背景  
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// 设置深度缓存  
	glClearDepth(1.0);
	// 启用深度测试  
	glEnable(GL_DEPTH_TEST);
	// 所作深度测试的类型  
	glDepthFunc(GL_LEQUAL);
	// 告诉系统对透视进行修正  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);



}
void InitShader()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "../shaders/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "../shaders/triangles.frag" },
		{ GL_NONE, NULL },
	};
	GLuint program = Program::Load(shaders);
	glUseProgram(program);
}
void resizeGL(GLFWwindow*, int w, int h)
{
	// 防止窗口大小变为0  
	if (h == 0)
	{
		h = 1;
	}
	// 重置当前的视口  
	glViewport(0, 0, (GLint)w, (GLint)h);
	// 选择投影矩阵  
	glMatrixMode(GL_PROJECTION);
	// 重置投影矩阵  
	glLoadIdentity();
	// 设置视口的大小  
	//gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	ActiveEngine::aeMat4f ad;
	ad.Perspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	ActiveEngine::aeMat4f regid;
	regid.Translate(-1, 0, -5); // 相机位置

	ad = ad*regid;
	float *mm = ad.get(); // 矩阵构成
	glMultMatrixf(mm);

	// 选择模型观察矩阵  

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	return;
}

void  drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	ActiveEngine::aeMat4f regid;
	regid.Translate(0, 0, -2); // 相机位置
	float *CamPos = regid.get(); // 矩阵构成
	//glMultMatrixf(CamPos);
	glLoadMatrixf(CamPos);

	// Reset The View  
	//glTranslatef(0.0f, 0.0f, -5.0f);
	//绘制一个正方形  
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();

}
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

	/* Loop until the user closes the window */
	//	GLuint vertexShader;
	//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//	glShaderSource(vertexShader, 1, &vertexShaderSource, null);
	//	glCompileShader(vertexShader);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 传递消息鼠标键盘等

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawScene();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

