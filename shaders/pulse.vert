#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 vTexture;

out vec3 outColor;

out vec2 TexCoord;

uniform mat4 model;
 
uniform mat4 view;
 
uniform mat4 projection;

void main()
{
	gl_Position = vec4(vPosition, 1.0f);
    outColor = vColor;
	TexCoord = vTexture;
}
