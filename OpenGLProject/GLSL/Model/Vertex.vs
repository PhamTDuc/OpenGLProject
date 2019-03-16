#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Position;
out vec3 Normal;
out VS_OUT{
	//vec2 texCoords;
	vec4 FragPosLightSpace;
} vs_out;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    TexCoords = aTexCoords;    
    Position=vec3(model * vec4(aPos, 1.0));
	Normal=mat3(transpose(inverse(model))) *aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(Position, 1.0);
	//gs_out.texCoords=aTexCoords;
	
}