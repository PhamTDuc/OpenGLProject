# version 330 core

const float offset=1.0f/400.0f;

in vec2 TexCoords;
uniform sampler2D ScreenTexture;
out vec4 FragColor;

void main(){
	vec4 texColor = texture(ScreenTexture, TexCoords);

	// Inverse Color
	//FragColor=vec4(vec3(1.0f-texture(ScreenTexture,TexCoords)),1.0f);

	//GrayScale Color
	//float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
	//FragColor=vec4(vec3(average),1.0f);

	//Kernel Effect
	vec2 offsets[9] ={
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    };

	float kernel[9]= {
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    };

	float kernel_blur[9]={
		 1.0 / 16, 2.0 / 16, 1.0 / 16,
		 2.0 / 16, 4.0 / 16, 2.0 / 16,
		 1.0 / 16, 2.0 / 16, 1.0 / 16  
	};

	float kernel_sharpen[9]={
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0,1.0
	};

	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(ScreenTexture, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel_sharpen[i];
    
    FragColor = vec4(col, 1.0f);
    
}