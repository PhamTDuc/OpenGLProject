#version 330 core


in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssao;
uniform mat4 projection;

const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);
int kernelSize = 64;
float radius = 0.4;
float bias = 0.045;

void main(){
	vec3 FragPos = texture(gPosition,TexCoords).rgb;
	vec3 Normal = normalize(texture(gNormal,TexCoords).rgb);

	//Blur ssao
	//Blur ssao
	vec2 texelSize = 1.0f/ vec2(textureSize(ssao,0));
	float ssaoVal = 0.0f;
	for(int x=-2;x<2;x++)
		for(int y=-2;y<2;y++)
		{
			vec2 offset = vec2(float(x),float(y))*texelSize;
			ssaoVal +=texture(ssao,TexCoords + offset).r;

		}
	
	ssaoVal/=16.0f;
	FragColor=vec4(vec3(ssaoVal), 1.0f);
}