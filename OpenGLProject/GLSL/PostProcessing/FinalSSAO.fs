#version 330 core
struct PointLight{
	vec3 pos;
    vec3 diffuse;

};


in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssao;
uniform mat4 view;
uniform PointLight lights[4];
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
	
	//Deferred Stage
	//Deferred Stage
	vec3 diffuse,specular;

	vec3 viewDir=normalize(-FragPos);
	for(int i = 0; i<4; i++){
		vec4 lightpos = view*vec4(lights[i].pos,1.0f); //From world space to view space
		float distance    = length(lightpos.xyz - FragPos);
			//Diffuse
			vec3 lightDir = normalize(lightpos.xyz - FragPos);
			float diff= max(dot(Normal, lightDir), 0.0f);
		
			//Specular
			vec3 reflectDir = normalize(reflect(-lightDir, Normal));  
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);


			//Attenuation
			float attenuation = 1.0 / (0.7f + 1.8 * distance + 
						20.0 * (distance * distance));    
			diff*=attenuation;
			spec*=attenuation;
			diffuse += lights[i].diffuse*diff;
			specular+=vec3(spec);
	}

	vec3 result;
	result=(diffuse+0.2f*ssaoVal)*vec3(0.9f,0.8f,0.9f) + specular*4.0f;
	FragColor=vec4(result,1.0f);

	
}