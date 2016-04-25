#version 430 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

struct Meterial
{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normalMap;
	float	  shininess;
};
uniform Meterial meterial;

out vec4 fColor;

in vec3 Normals;

in vec2 TexCoords;

in vec3 FragPosition;

uniform sampler2D floolTexture;

uniform sampler2D floolNormalMap;

void main()
{
	vec3 norm = normalize(Normals)*normalize(vec3(texture(floolNormalMap, TexCoords)));
	// ·½Ïò¹â
	vec3 lightDir =  normalize(light.position - FragPosition);
	
	// diffuse
	float diff = max(dot(norm,lightDir),0.0); 
	vec3 diffuse = (diff*vec3(texture(floolTexture, TexCoords))) ;

    fColor = dot(norm,lightDir)*texture(floolTexture,TexCoords);//
}