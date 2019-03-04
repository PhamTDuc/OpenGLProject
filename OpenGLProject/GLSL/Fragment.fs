#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;  
    sampler2D emission;  
    float shininess;
}; 

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct SpotLight{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerCutOff;
    float outerCutOff;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform bool blinn;
uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.4);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
	float spec;
	if (blinn)
	{	
		 vec3 halfDir=normalize(lightDir+viewDir);
		 spec = pow(max(dot(norm, halfDir), 0.0), 16.0f);
	}
	else
	{	
		vec3 reflectDir = reflect(-lightDir, norm);  
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);
	}
    
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // emission
    //vec3 emission = texture(material.emission, TexCoords).rgb;

    // Attenuation for Pointlight and Spotlight
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                    light.quadratic * (distance * distance));    

    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;

    // More attributes for Spotlight
    float theta     = dot(-lightDir, normalize(light.direction));
    float epsilon   = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
    
    diffuse  *= intensity;
    specular *= intensity;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
} 