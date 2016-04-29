#version 430 core

in vec3 outColor;

in vec2 TexCoord;

out vec4 fColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D screenTexture;

uniform int windowWidth; //传入窗口尺寸
uniform int windowHeight;
uniform float dTimer; // cpu  传入时间

void main()
{
  float lsin = sin(dTimer);
  // fColor =  vec4(outColor, 1.0); 
  // fColor = texture2D(ourTexture1, TexCoord);//* vec4(outColor, 1.0f);
  float borderX = (min(windowWidth,windowHeight)/2)*(1.0+lsin)/2;
  
  vec2 rad = vec2(gl_FragCoord.x - (windowWidth/2),gl_FragCoord.y -(windowHeight/2));

  float radlength = length(rad);// 获取长度
 
  if(radlength < borderX &&  radlength > borderX-12)
  {
	 fColor =  vec4(outColor, 1.0); 
  }
  else
	fColor = mix(texture2D(ourTexture1, TexCoord),texture2D(ourTexture2, vec2(TexCoord.x,1-TexCoord.y)),0.25);
}