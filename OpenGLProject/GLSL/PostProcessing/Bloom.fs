# version 330 core


in vec2 TexCoords;
uniform sampler2D ScreenTexture;
uniform sampler2D BloomTexture;
out vec4 FragColor;

vec3 Uncharted2Tonemap(vec3 x){	
	const float A = 0.15;
	const float B = 0.50;
	const float C = 0.10;
	const float D = 0.20;
	const float E = 0.02;
	const float F = 0.30;


	return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
} 




void main(){
	//Gamma
	float gamma=2.2f;
	vec3 texColor = texture(ScreenTexture, TexCoords).rgb;
	vec3 texBloom = texture(BloomTexture, TexCoords).rgb;
	texColor+=texBloom;
	

	//Reinhard tone mapping
	//Reinhard tone mapping
	//texColor*=16; //Hard exposure adjustment

	vec3 mapped=texColor/(texColor+vec3(1.0f));

	//Exposure tone mapping
	//Exposure tone mapping
	float exposure=-0.05;
	vec3 expose=vec3(1.0f)-exp(-texColor*exposure);



	//Uncharted2Tonemapping
	//Uncharted2Tonemapping
	float ExposureBias = 2.0f;
	vec3 curr = Uncharted2Tonemap(ExposureBias*texColor);

	vec3 W=vec3(11.2f);
	vec3 whiteScale = vec3(1.0f)/Uncharted2Tonemap(W);
	vec3 unchart = curr*whiteScale;

	//Gamma correction
	vec3 result=mapped;
	result=pow(result,vec3(1.0f/gamma));
	FragColor=vec4(result,1.0f);

	

	
    
}