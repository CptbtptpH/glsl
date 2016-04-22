#version 430 core

struct Light
{
	vec3 position;
};
uniform Light light;

out vec4 fColor;

in vec3 Normals;

in vec2 TexCoords;

in vec3 FragPosition;

uniform sampler2D floolTexture;

void main()
{
	vec3 norm = normalize(Normals);
	// ·½Ïò¹â
	vec3 lightDir =  normalize(light.position - FragPosition);
	
	// diffuse
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = (diff*vec3(texture(floolTexture, TexCoords))) ;

    fColor = dot(norm,lightDir)*vec4( 1.0f,0.0f,1.0f,1.0f);  //vec4( diffuse,1.0f);//texture(floolTexture,TexCoords);//
}