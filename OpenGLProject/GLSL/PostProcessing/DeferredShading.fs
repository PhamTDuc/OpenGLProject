#version 330 core

struct PointLight{
	vec3 pos;
    vec3 diffuse;
    //vec3 specular;
	//float constant;
	//float linear;
	//float quadratic;
	float radius;
};

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform vec3 viewPos;
uniform PointLight lights[4];


void main(){
	vec3 FragPos = texture(gPosition,TexCoords).rgb;
	vec3 Normal = texture(gNormal,TexCoords).rgb;
	vec3 Color = texture(gColorSpec,TexCoords).rgb;
	float Specular = texture(gColorSpec, TexCoords).a;
	

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	vec3 viewDir = normalize(viewPos-FragPos);


	for(int i = 0; i<4; i++){
		float distance    = length(lights[i].pos - FragPos);
			//Diffuse
			vec3 lightDir = normalize(lights[i].pos - FragPos);
			float diff= max(dot(Normal, lightDir), 0.0f);
		
			//Specular
			vec3 reflectDir = normalize(reflect(-lightDir, Normal));  
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0)*4.0f;


			//Attenuation
			float attenuation = 1.0 / (0.7f + 1.8 * distance + 
						20.0 * (distance * distance));    
			diff*=attenuation;
			spec*=attenuation;
			diffuse += lights[i].diffuse*diff;
			specular+=vec3(spec*Specular);
	}

	vec3 result;
	result=diffuse*Color+specular;

	//ToneMapping
	//ToneMapping
	float exposure = 0.5f;
	result = vec3(1.0f) - exp(-exposure*result);

	//GammaCorrection
	//GammaCorrection
	float gamma = 2.2f;
	result = pow(result,vec3(1.0f/gamma));
	FragColor=vec4(result,1.0f);

}