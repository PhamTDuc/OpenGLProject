#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D ambient1; //Using ambient map as reflection map
uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(Position-camPos);

	//Reflection
	vec3 R=reflect(I,normalize(Normal));

	//Refraction
	float ratio = 1.00 / 1.52;
	//vec3 R = refract(I, normalize(Normal), ratio);

	vec3 reflection=texture(skybox,R).rgb*texture(ambient1,TexCoords).rgb;
	vec3 diffuse=texture(diffuse1,TexCoords).rgb;
	FragColor=vec4(reflection+diffuse,1.0f);
 
}