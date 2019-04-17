#version 330 core


in vec2 TexCoords;
out float FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform vec3 samples[64];
uniform mat4 projection;

const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);
int kernelSize = 64;
float radius = 0.2f;
float bias = 0.1;

void main(){
	vec3 FragPos = texture(gPosition,TexCoords).rgb;
	vec3 Normal = normalize(texture(gNormal,TexCoords).rgb);
	vec3 randomVec = normalize(texture(texNoise, TexCoords*noiseScale).xyz);
	

	//Create TBN from tangent-space to view-space
	//Create TBN from tangent-space to view-space
	vec3 tangent = normalize(randomVec - Normal * dot(randomVec,Normal));
	vec3 bitangent = cross(Normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, Normal);
	
	//Iterate over the sample kernel and calculate occlusion factor
	//Iterate over the sample kernel and calculate occlusion factor
	float occlusion = 0.0f;
	for (int i = 0; i < kernelSize; i++)
	{
		vec3 sample = TBN * samples[i];
		sample = FragPos + sample * radius;

		//Project sample position (to sample texture) to get position on screen/texture
		vec4 offset = vec4(sample, 1.0f);
		offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
		//Get sample Depth
		float sampleDepth = texture(gPosition, offset.xy).z;

		//Range check & accumulate
		float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(FragPos.z - sampleDepth));
		//rangeCheck = 1.0f;
		occlusion += (sampleDepth >= sample.z + bias ? 1.0f:0.0f) * rangeCheck;
	}

	occlusion = 1.0f - (occlusion / kernelSize);
	//ssao=occlusion;
	//SSAO Checker
	//SSAO Checker
	vec3 checker = vec3(occlusion);
	checker = pow(checker,vec3(2.0f));
	FragColor=occlusion;
}