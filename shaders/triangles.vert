#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 3) in vec3 normal;
uniform mat4 model;
 
uniform mat4 view;
 
uniform mat4 projection;
out vec3 Normal;
out vec3 FragPos;
void main()
{
	gl_Position = projection*view*model * vec4(vPosition, 1.0f);
	FragPos = vec3(model*vec4(vPosition,1.0f)); 
	Normal = normal;
}
