#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
out vec3 outColor;

uniform mat4 model;
 
uniform mat4 view;
 
uniform mat4 projection;
uniform mat4 MVP;
void main()
{
	gl_Position = MVP * vec4(vPosition, 1.0f);
    outColor = vColor;
}
