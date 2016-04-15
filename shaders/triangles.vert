#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
out vec3 outColor;

uniform mat4 model;
 
uniform mat4 view;
 
uniform mat4 projection;

void main()
{
 //   gl_Position = vec4(vPosition,1.0f);
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
    outColor = vColor;

	
}
