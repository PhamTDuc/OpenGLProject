#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;
const float PI = 3.14159265359;

float VanDerCorpus(uint n, uint base)
{
    float invBase = 1.0 / float(base);
    float denom   = 1.0;
    float result  = 0.0;

    for(uint i = 0u; i < 32u; ++i)
    {
        if(n > 0u)
        {
            denom   = mod(float(n), 2.0);
            result += denom * invBase;
            invBase = invBase / 2.0;
            n       = uint(float(n) / 2.0);
        }
    }

    return result;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), VanDerCorpus(i, 2u));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}  
float DistributionGGX(float NdotH, float roughness) 
{
	float a = roughness * roughness;
	float a2 = a*a;
	float NdotH2= NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2*(a2-1.0f)+1.0f);
	denom = PI * denom * denom;
	denom=max(denom,0.001);
	
	return num / denom; 
}


void main()
{		
    vec3 N = normalize(localPos);    
    vec3 R = N;
    vec3 V = R;

<<<<<<< HEAD
    const uint SAMPLE_COUNT = 128;
=======
    const uint SAMPLE_COUNT = 1024;
>>>>>>> 24a3f4346f25ca3964bdaa6dde25bb42d7a640c5
    float totalWeight = 0.0f;   
    vec3 prefilteredColor = vec3(0.0f);     
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);
		float NdotH = max(dot(N,H),0.0);
		float HdotV = max(dot(H,V),0.0);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
			float D=DistributionGGX(NdotH,roughness);
			float pdf=( D + NdotH/( 4.0*HdotV ))+ 0.0001;

			float resolution = 512.0f;
			float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
			float saSample = 1.0 / (SAMPLE_COUNT*pdf + 0.0001);
			float mipLevel = (roughness == 0.0) ? 0.0 : 0.5 * log2(saSample/saTexel);
			


			prefilteredColor += textureLod(environmentMap, L ,mipLevel).rgb * NdotL;
<<<<<<< HEAD
=======
            totalWeight      += NdotL;
>>>>>>> 24a3f4346f25ca3964bdaa6dde25bb42d7a640c5
        }
    }
    prefilteredColor = prefilteredColor / SAMPLE_COUNT;

    FragColor = vec4(prefilteredColor, 1.0);
}  