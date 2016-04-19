#version 430 core

in vec3 outColor;

in vec2 TexCoord;

out vec4 fColor;

uniform sampler2D ourTexture;

void main()
{
  // fColor =  vec4(outColor, 1.0); 
   fColor = texture(ourTexture, TexCoord)* vec4(outColor, 1.0f);;
 
}