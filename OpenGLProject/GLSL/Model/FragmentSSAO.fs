#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

//out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 N;  
  
uniform sampler2D diffuse1;
uniform sampler2D specular1;

void main()
{    
	gPosition=FragPos;
	gNormal=N;
	//gColorSpec.rgb=texture(diffuse1, TexCoords).rgb;
	//gColorSpec.a=texture(specular1, TexCoords).r;
	
}