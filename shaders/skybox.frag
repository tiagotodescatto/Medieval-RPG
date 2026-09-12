#version 460 core

in vec3 LocalPos;

out vec4 FragColor;

uniform samplerCube environmentMap;

void main()
{
    FragColor = texture(environmentMap, LocalPos);
}