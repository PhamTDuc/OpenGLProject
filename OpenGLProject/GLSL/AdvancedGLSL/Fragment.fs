#version 330 core

out vec4 FragColor;
layout (std140) uniform MatricesBlock{
	                // base alignment  // aligned offset
    float value;     // 4               // 0 
    vec3 vector;     // 16              // 16  (must be multiple of 16 so 4->16)
    mat4 matrix;     // 16              // 32  (column 0)
                     // 16              // 48  (column 1)
                     // 16              // 64  (column 2)
                     // 16              // 80  (column 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
};



void main(){
	//if(gl_FrontFacing)
    //    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //else
    //    FragColor = vec4(0.0, 1.0, 0.0, 1.0);     
	FragColor=vec4(0.0f,1.0f,0.0f,1.0f);
}