#version 430 core
uniform vec3 lightColor;

out vec4 fColor;
 
uniform vec3 objectColor;

uniform vec3 lightPos;

in vec3 Normal; 
 
in vec3 FragPos;

void main()
{
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

   

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * lightColor;
	 vec3 result = (ambient + diffuse) * objectColor;
    fColor = vec4(result, 1.0f);
}