#ifndef __VGL_H__
#define __VGL_H__
 
#define GLEW_STATIC

#include "glew\GL\glew.h"
#include "glfw\glfw3.h"
#include "FreeImage.h"
//#include "gl/glu.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#ifdef _MSC_VER
#pragma comment(lib,"FreeImage/FreeImage.lib")
#pragma comment(lib,"opengl32.lib")
//#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"assimp/assimpd.lib")
#pragma comment(lib,"glfwLib64/glfw3.lib")
#pragma comment(lib,"glfwLib64/glfw3dll.lib")
#pragma comment(lib,"glewLib64/glew32s.lib")
#pragma comment(lib,"glewLib64/glew32.lib")
#endif
 
#endif /* __VGL_H__ */
