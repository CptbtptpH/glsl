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
	float cutOff;// �۹��
};
uniform Light light;

out vec4 fColor;
 
uniform vec3 objectColor;

uniform vec3 viewPos;

in vec3 Normal; 
 
in vec3 FragPos;

in vec2 TexCoords;

void main()
{
	float ambientStrength = 0.2f;

	float distance = length(light.position - FragPos);
	float attenuation = 1/(light.constant + light.linear*distance+light.quadratic*(distance*distance));
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

	vec3 norm = normalize(Normal);
	// �����
	vec3 lightDir =  normalize(light.position - FragPos);
	
	// diffuse
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = (diff*vec3(texture(material.diffuse, TexCoords)))* light.diffuse;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = vec3(texture(material.specular, TexCoords))* spec * light.specular;
	vec3 result = ambient + diffuse+specular;

    fColor = vec4(result*attenuation, 1.0f);
}