#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;  
  
in VS_OUT{
	//vec2 texCoords;
	vec4 FragPosLightSpace;
} fs_in;
struct DirectionLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D diffuse1;
uniform sampler2D specular1;
uniform sampler2D shadowMap;
uniform DirectionLight light;
uniform vec3 viewPos;



float LinearizeDepth(float depth,float near,float far){
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

float ShadowCalculation(vec4 fragPosLightSpace,float bias){
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;


	//PCF percentage-closer filtering
	//PCF percentage-closer filtering
	float shadow=0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x)
	{
		for(int y = -2; y <= 2; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 16.0;
    // check whether current frag pos is in shadow
	if(projCoords.z > 1.0)
        shadow = 0.0;

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
    vec3 lightDir = normalize(light.direction);
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
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace,bias);       
	vec3 result = (ambient + (1-shadow)*(diffuse + specular))*texColor;
	
	// Calcualte result without shadow
	//vec3 result=(ambient+diffuse+specular)*texColor;

	FragColor = vec4(result, 1.0);
}