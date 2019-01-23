#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse1;
uniform sampler2D specular1;

float LinearizeDepth(float depth,float near,float far){
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
void main()
{    
    vec4 texColor = texture(diffuse1, TexCoords);
    //if(texColor.a < 0.2)
      //  discard;
    FragColor = texColor;
		//float depth=LinearizeDepth(gl_FragCoord.z,0.1f,50.0f)/50.0f;
		//FragColor=vec4(vec3(depth),1.0f);
		//FragColor=vec4(1.0f);
}