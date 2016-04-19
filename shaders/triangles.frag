#version 430 core
uniform vec3 lightColor;

out vec4 fColor;
 
uniform vec3 objectColor;

 
void main()
{
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    fColor = vec4(result, 1.0f);
}