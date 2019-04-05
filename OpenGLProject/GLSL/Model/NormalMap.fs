#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 N;  
  
in VS_OUT{
	//vec2 texCoords;
	//vec3 T;
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
	normal = normal*2.0f -1.0f;
	normal=normalize(fs_in.TBN*normal);
	

	// ambient
    vec3 ambient = vec3(0.0f);
    
    // diffuse 
    vec3 lightDir = normalize(light.pos-FragPos);
    float diff = max(dot(normal,lightDir), 0.0);
    vec3 diffuse = light.diffuse*diff ;  
	
	// specular
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, normal));  
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = spec*light.specular;  

	// combine
	// combine

	  // Attenuation for Pointlight and Spotlight
    float distance    = length(light.pos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                    light.quadratic * (distance * distance));    

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
	

	vec3 result;
	// Calcualte result without shadow
	result=(ambient+diffuse)*texColor+specular;

	

	
	FragColor = vec4(result, 1.0f);

	//For bloom Effect
	//For bloom Effect
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 12.0f)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}