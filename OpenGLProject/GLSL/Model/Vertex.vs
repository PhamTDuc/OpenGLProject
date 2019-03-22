#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 Position;
out vec3 Normal;
out VS_OUT{
	//vec2 texCoords;
	vec4 FragPosLightSpace;
	mat3 TBN;
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


	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);
	
}