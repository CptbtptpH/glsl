//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

//----------------------------------------------------------------------------
//

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

struct Program
{
	static  char*	readShaderSource(const char* shaderFile)
	{
		FILE* fp;// = fopen(shaderFile, "rb");
		fopen_s(&fp, shaderFile, "rb");

		if (fp == NULL) { return NULL; }

		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);

		fseek(fp, 0L, SEEK_SET);
		char* buf = new char[size + 1];
		fread(buf, 1, size, fp);

		buf[size] = '\0';
		fclose(fp);
		return buf;
	}

	static GLuint Load(ShaderInfo* shadersInfo)
	{
		if (nullptr == shadersInfo)
		{
			return -1;
		}
		GLuint prog = glCreateProgram();
		int index = 0;
		while (shadersInfo[index].type != GL_NONE)
		{
			char* souce = readShaderSource(shadersInfo[index].filename);
			printf("\n\n------------------shader文本信息----------------------\n");
			printf(souce);
			AttachShader(prog, shadersInfo[index].type, souce);
			index++;
		}
		glLinkProgram(prog);
		CheckStatus(prog);
		return prog;
	}

private:
	static void CheckStatus(GLuint obj)
	{
		GLint status = GL_FALSE;
		if (glIsShader(obj)) 
			glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
		if (glIsProgram(obj)) 
			glGetProgramiv(obj, GL_LINK_STATUS, &status);
		if (status == GL_TRUE) 
			return;
		GLchar log[1 << 15] = { 0 };
		if (glIsShader(obj))
			glGetShaderInfoLog(obj, sizeof(log), NULL, log);
		if (glIsProgram(obj)) 
			glGetProgramInfoLog(obj, sizeof(log), NULL, log);
		std::cerr << log << std::endl;
		exit(-1);
	}

	static void AttachShader(GLuint program, GLenum type, const char* src)
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);
		CheckStatus(shader);
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}
};

//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __LOAD_SHADERS_H__
