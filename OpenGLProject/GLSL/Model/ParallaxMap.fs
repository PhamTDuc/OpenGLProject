#version 330 core
out vec4 FragColor;

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
uniform sampler2D depthMap1;
uniform PointLight light;
uniform vec3 viewPos;


//ParallaxMapping Implementation (usually used for ground or wall)
//ParallaxMapping Implementation (usually used for ground or wall)
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDirTangent)
{	
	const float height_scale=0.05;
    float height =  texture(depthMap1, texCoords).r;    
    vec2 p = viewDirTangent.xy / viewDirTangent.z * (height * height_scale);
    //return texCoords - p;    

	// number of depth layers
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDirTangent)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDirTangent.xy * height_scale; 
    vec2 deltaTexCoords = P / numLayers;
	// get initial values
	vec2  currentTexCoords     = texCoords;
	float currentDepthMapValue = texture(depthMap1, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(depthMap1, currentTexCoords).r;  
		// get depth of next layer
		currentLayerDepth += layerDepth;  
	}

	//return currentTexCoords; //Step ParallaxMapping
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(depthMap1, prevTexCoords).r - currentLayerDepth + layerDepth;
 
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;  
} 


void main()
{    
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 viewDirTangent=transpose(fs_in.TBN)*viewDir;
	vec2 texCoords=ParallaxMapping(TexCoords, viewDirTangent);
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;

    vec3 texColor = texture(diffuse1, texCoords).rgb;
	vec3 normal = texture(normalMap1,texCoords).rgb;
	normal = normal*2.0f -1.0f;
	normal=normalize(fs_in.TBN*normal);
	

	// ambient
    vec3 ambient = vec3(0.0f);
    
    // diffuse 
    vec3 lightDir = normalize(light.pos-FragPos);
    float diff = max(dot(normal,lightDir), 0.0f);
    vec3 diffuse = light.diffuse*diff ;  
	
	// specular
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

	

}