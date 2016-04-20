#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
uniform mat4 model;
 
uniform mat4 view;
 
uniform mat4 projection;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
void main()
{
	gl_Position = projection*view*model * vec4(vPosition, 1.0f);
	FragPos =   vec3(model*vec4(vPosition,1.0f)); //
	Normal =vec3(model*vec4(normal,1.0f));// normal;
	TexCoords = texCoords;
}
