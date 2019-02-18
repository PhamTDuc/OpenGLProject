#version 330 core
out vec4 FragColor;
in VS_OUT2{
	vec3 color;
} gcolor_in;

void main()
{
    FragColor = vec4(gcolor_in.color,1.0f);
}  