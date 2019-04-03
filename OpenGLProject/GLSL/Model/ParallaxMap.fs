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
uniform sampler2D depthMap1;
uniform PointLight light;
uniform vec3 viewPos;


//Parallax Mapping is a great technique to boost the detail of your scene, 
//but does come with a few artifacts you'll have to consider when using it. 
//Most often parallax mapping is used on floor or wall-like surfaces 
//where it's not as easy to determine the surface's outline 
//and the viewing angle is most often roughly perpendicular to the surface. 
//This way the artifacts of Parallax Mapping aren't as noticeable and makes it an incredibly interesting technique for boosting your objects' details.

vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir)
{	
	const float height_scale=2.0f;
	float height =  texture(depthMap1, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    return texCoords - p;
}


vec2 SteepParallaxMapping(vec2 texCoords,vec3 viewDir){
	const float height_scale=0.5f;


	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  


    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
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
	//Disable the line below to Implement Parallax Occlusion Mapping
	//return currentTexCoords;

	//Implement Parallax Occlusion Mapping
	//Implement Parallax Occlusion Mapping
	
	// get texture coordinates before collision (reverse operations)
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
	vec3 viewDir = normalize(fs_in.TBN*(viewPos - Position));

	//Implement ParallaxMapping
	//Implement ParallaxMapping
	vec2 parallax_texCoords=TexCoords;
	//vec2 parallax_texCoords = SteepParallaxMapping(TexCoords,  viewDir);
	if(parallax_texCoords.x > 1.0 || parallax_texCoords.y > 1.0 || parallax_texCoords.x < 0.0 || parallax_texCoords.y < 0.0)
		discard;
    vec3 texColor = texture(diffuse1, parallax_texCoords).rgb;
	vec3 normal = texture(normalMap1,parallax_texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	

	// ambient
    vec3 ambient = light.ambient;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(fs_in.TBN*(light.pos-Position));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse*diff ;  
	
	// specular
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
	result=(ambient+diffuse)*texColor+specular;

	//Testing normalmap 
	//result=transpose(fs_in.TBN)*normal;

	FragColor = vec4(result, 1.0f);
}