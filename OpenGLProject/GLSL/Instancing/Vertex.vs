#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform vec2 offsets[40];

void main(){
	vec2 offset=offsets[gl_InstanceID];

	gl_Position=projection*view*vec4(aPos+vec3(offset,0.0f),1.0f);

}