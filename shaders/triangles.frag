#version 430 core

struct Material 
{ 
    vec3 ambient; 
    sampler2D diffuse; 
    sampler2D specular; 
    float shininess; 
}; 
uniform Material material;

//uniform vec3 lightColor;

struct Light 
{ 
    vec3 position; 
	vec3 direction;
    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 
	float constant;
	float linear;
	float quadratic;
	float cutOff;// ¾Û¹âµÆ
};
uniform Light light;

out vec4 fColor;
 
uniform vec3 objectColor;

uniform vec3 viewPos;

in vec3 Normal; 
 
in vec3 FragPos;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    fColor = vec4(texture(texture_diffuse1, TexCoords));
}