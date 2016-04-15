// shader_test1.cpp : �������̨Ӧ�ó������ڵ㡣
//

// glfw_glew_test1.cpp : �������̨Ӧ�ó������ڵ㡣
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

	// ���õ�ǰ���ӿ�  
	glViewport(0, 0, (GLint)w, (GLint)h);

	// ѡ��ͶӰ����  
	glMatrixMode(GL_PROJECTION);
	// ����ͶӰ����  
	glLoadIdentity();

	// �����ӿڵĴ�С  
	//gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	ActiveEngine::aeMat4f ad;
	ad.Perspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	ActiveEngine::aeMat4f regid;
	regid.Translate(-1, 0, -5); // ���λ��

	ad = ad*regid;

	float *mm = ad.get(); // ���󹹳�
	glMultMatrixf(mm);

	// ѡ��ģ�͹۲����  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ������Ӱƽ��  
	glShadeModel(GL_SMOOTH);
	// ��ɫ����  
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// ������Ȼ���  
	glClearDepth(1.0);
	// ������Ȳ���  
	glEnable(GL_DEPTH_TEST);
	// ������Ȳ��Ե�����  
	glDepthFunc(GL_LEQUAL);
	// ����ϵͳ��͸�ӽ�������  
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
	// ��ֹ���ڴ�С��Ϊ0  
	if (h == 0)
	{
		h = 1;
	}
	// ���õ�ǰ���ӿ�  
	glViewport(0, 0, (GLint)w, (GLint)h);
	// ѡ��ͶӰ����  
	glMatrixMode(GL_PROJECTION);
	// ����ͶӰ����  
	glLoadIdentity();
	// �����ӿڵĴ�С  
	//gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	ActiveEngine::aeMat4f ad;
	ad.Perspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	ActiveEngine::aeMat4f regid;
	regid.Translate(-1, 0, -5); // ���λ��

	ad = ad*regid;
	float *mm = ad.get(); // ���󹹳�
	glMultMatrixf(mm);

	// ѡ��ģ�͹۲����  

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	return;
}

void  drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	ActiveEngine::aeMat4f regid;
	regid.Translate(0, 0, -2); // ���λ��
	float *CamPos = regid.get(); // ���󹹳�
	//glMultMatrixf(CamPos);
	glLoadMatrixf(CamPos);

	// Reset The View  
	//glTranslatef(0.0f, 0.0f, -5.0f);
	//����һ��������  
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
		glfwPollEvents(); // ������Ϣ�����̵�

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawScene();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

