#version 430 core

in vec3 outColor;

in vec2 TexCoord;

out vec4 fColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D screenTexture;

void main()
{
  // fColor =  vec4(outColor, 1.0); 
  // fColor = texture2D(ourTexture1, TexCoord);//* vec4(outColor, 1.0f);
  fColor = mix(texture2D(ourTexture1, TexCoord),texture2D(ourTexture2, vec2(TexCoord.x,1-TexCoord.y)),0.25);
}