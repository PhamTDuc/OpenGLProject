#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;  
  
in VS_OUT{
	//vec2 texCoords;
	vec4 FragPosLightSpace;
	mat3 TBN;
} fs_in;
struct PointLight{
	vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D diffuse1;
uniform sampler2D normalMap1;
uniform PointLight light;
uniform vec3 viewPos;




void main()
{    
    vec3 texColor = texture(diffuse1, TexCoords).rgb;
	vec3 normal = texture(normalMap1,TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	

	// ambient
    vec3 ambient = light.ambient;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(fs_in.TBN*(light.pos-Position));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse*diff ;  
	
	// specular
	vec3 viewDir = normalize(fs_in.TBN*(viewPos - Position));
	vec3 reflectDir = normalize(reflect(-lightDir, normal));  
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = spec*light.specular;  

	// combine
	// combine

	  // Attenuation for Pointlight and Spotlight
    float distance    = length(light.pos - Position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                    light.quadratic * (distance * distance));    

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
	

	vec3 result;
	// Calcualte result without shadow
	//result=(ambient+diffuse)*texColor+specular;

	//Testing normalmap 
	result=transpose(fs_in.TBN)*normal;

	FragColor = vec4(result, 1.0f);
}