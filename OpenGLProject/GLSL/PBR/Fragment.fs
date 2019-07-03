#version 330 core
out vec4 FragColor;

struct Light{
	vec3 pos;
	vec3 diffuse;
};


in vec2 TexCoords;
in vec3 FragPos; 
in vec3 Normal;

uniform Light lights[16];
uniform vec3 viewPos;
uniform vec3 color;
uniform bool useTexture;
uniform sampler2D albedoMap;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform samplerCube irradianceMap;

const float PI = 3.14159265359;
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
	return F0 + (1-F0) * pow(1.0f - cosTheta, 5.0f);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) 
{
	float a = roughness * roughness;
	float a2 = a*a;
	float NdotH = max(dot(N,H),0.0f);
	float NdotH2= NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2*(a2-1.0f)+1.0f);
	denom = PI * denom * denom;
	denom=max(denom,0.001);
	
	return num / denom; 
}

float GeometrySchlickGGX(float NdotV, float roughness) 
{
	float r = roughness+1.0f;
	float k = (r*r)/8.0f;

	float num = NdotV;
	float denom = NdotV*(1.0f-k) +k;
	denom=max(denom,0.001);
	
	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N,V),0.0f);
	float NdotL = max(dot(N,L),0.0f);
	float ggx1 = GeometrySchlickGGX(NdotL,roughness);
	float ggx2 = GeometrySchlickGGX(NdotV,roughness);

	return ggx1*ggx2;
}

void main()
{
	vec3 N = normalize(Normal);
	vec3 V = normalize(viewPos - FragPos).rgb;

	vec3 albedo;
	if(useTexture)
		albedo=texture(albedoMap,TexCoords).rgb;
	else
		albedo=color;


	//Fresnel workflow
	vec3 F0 = vec3(0.04f);
	F0 = mix(F0, albedo, metallic);


	vec3 Lo=vec3(0.0f);
	for(int i = 0; i<16;i++)
	{
		vec3 L = normalize(lights[i].pos - FragPos);
		vec3 H = normalize(V+L);

		float distance = length(lights[i].pos-FragPos);
		float attenuation = 1.0f / (distance*distance);
		vec3 radiance = lights[i].diffuse * attenuation;

		//Cook-Torrance BRDF
		float D= DistributionGGX(N,H,roughness);
		float G= GeometrySmith(N,V,L, roughness);
		vec3 F = fresnelSchlick(max(dot(H,V),0.0f),F0);

		vec3 kS=F;
		vec3 kD=vec3(1.0f)-kS;
		kD*=1.0f- metallic;

		vec3 numerator = D*G*F;
		float denominator = 4 * max(dot(N,V),0.0f)* max(dot(N, L), 0.0);
		vec3 specular = numerator /max(denominator,0.001);

		//Add to outgoing radiance Lo
		//Add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
	}


	//Ambient
	vec3 kS=fresnelSchlickRoughness(max(dot(N,V),0.0f),F0,roughness);
	vec3 kD=1.0 - kS;
	kD *=1.0 - metallic;
	vec3 irradiance = texture(irradianceMap,N).rgb;
	vec3 ambient = (kD * irradiance* ao) * albedo;
    vec3 color =Lo + ambient;
	
	//Color Correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1.0);
}
