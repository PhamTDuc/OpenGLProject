#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1 ) in vec2 aOffset;

uniform mat4 projection;
uniform mat4 view;

void main(){

	gl_Position=projection*view*vec4(aPos+vec3(aOffset,0.0f),1.0f);

}