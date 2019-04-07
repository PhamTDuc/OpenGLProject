#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;
 
out vec2 TexCoords;
out vec3 FragPos;
out vec3 N;
out VS_OUT{
	//vec2 texCoords;
	//vec3 T;
	mat3 TBN;
} vs_out;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    TexCoords = aTexCoords;    
    FragPos=vec3(model * vec4(aPos, 1.0f));


	
	//vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	//gs_out.texCoords=aTexCoords;


	mat3 normalMatrix=mat3(transpose(inverse(model)));
	N      = normalize(normalMatrix * aNormal);
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = normalize(normalMatrix * aBiTangent);

	
	
	//T = normalize(T - dot(T, N) * N);
	//then retrieve perpendicular vector B with the cross product of T and N
	//B = cross(T, N);
	// re-orthogonalize T with respect to N
	if (dot(cross(T,B), N) < 0.0f){
		T=-T;
	}

	
	//vs_out.T=B;

	vs_out.TBN=mat3(T,B,N);

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}