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
};

uniform sampler2D diffuse1;
uniform sampler2D normalMap1;
uniform PointLight light;
uniform vec3 viewPos;




void main()
{    
    vec3 texColor = texture(diffuse1, TexCoords).rgb;
	vec3 normal = texture(normalMap1,TexCoords).rgb;
	//normal = normalize(normal * 2.0 - 1.0);
	//normal = normalize(fs_in.TBN*normal);
	// ambient
    vec3 ambient = light.ambient;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos-Position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff ;  
	
	// specular
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = normalize(reflect(-lightDir, normal));  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);
	vec3 specular = spec*light.specular;  

	// combine
	// combine

	// Calcualte result without shadow
	vec3 result=(diffuse+ambient+specular)*texColor;

	FragColor = vec4(normal, 1.0);
}