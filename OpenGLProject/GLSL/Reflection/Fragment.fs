#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

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


    FragColor = vec4(texture(skybox, R).rgb*0.5f, 1.0f);
	
}