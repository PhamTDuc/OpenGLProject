#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

//out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 N;  
  
in VS_OUT{
	//vec2 texCoords;
	//vec3 T;
	mat3 TBN;
} fs_in;

uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D normalMap1;


void main()
{    
	vec3 normal = texture(normalMap1,TexCoords).rgb;
	normal = normal*2.0f -1.0f;
	normal=normalize(fs_in.TBN*normal);
	
	gPosition=FragPos;
	gNormal=normal;
	gColorSpec.rgb=texture(diffuse1, TexCoords).rgb;
	gColorSpec.a=texture(specular1, TexCoords).r;
	
}