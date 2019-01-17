#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse1;
uniform sampler2D specular1;

void main()
{    
        FragColor = texture(diffuse1, TexCoords);
}