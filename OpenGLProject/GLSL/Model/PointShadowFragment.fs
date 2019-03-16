#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;  
  
in VS_OUT{
	//vec2 texCoords;
	vec4 FragPosLightSpace;
} fs_in;
struct PointLight{
	vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform samplerCube shadowMap;
uniform PointLight light;
uniform vec3 viewPos;

uniform float far_plane;


float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - light.pos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowMap,fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

	//shadow=closestDepth / far_plane;// Check ShadowMap
    return shadow;
}  


void main()
{    
    vec3 texColor = texture(diffuse1, TexCoords).rgb;
    //if(texColor.a < 0.2)
      //  discard;
		//FragColor = texColor;
		//float depth=LinearizeDepth(gl_FragCoord.z,0.1f,50.0f)/50.0f;
		//FragColor=vec4(vec3(depth),1.0f);
		//FragColor=vec4(1.0f);

	// ambient
    vec3 ambient = light.ambient;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos-Position);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff ;  
	
	// specular
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);
	vec3 specular = spec*light.specular;  

	// combine
	// combine

	// Calculate result with Shadow
	float bias=max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);  
	float shadow = ShadowCalculation(Position);       
	vec3 result = (ambient + (1-shadow)*(diffuse + specular))*texColor;
	
	// Calcualte result without shadow
	//vec3 result=(ambient+diffuse+specular)*texColor;

	//FragColor=vec4(vec3(shadow),1.0f); //Check ShadowMap
	FragColor = vec4(result,1.0f);
}