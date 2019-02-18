#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

out VS_OUT2{
	vec3 color;
} gcolor_out;

const float MAGNITUDE = 0.2;

void GenerateLine(int index)
{
	gcolor_out.color=vec3(1.0f,index/3.0f,1.0f);
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    //GenerateLine(0); // first vertex normal
	//GenerateLine(1); // second vertex normal
	GenerateLine(2); // third vertex normal
}