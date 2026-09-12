#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 LocalPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    LocalPos = aPos;

    mat4 rotationView = mat4(mat3(view));

    vec4 pos = projection * rotationView * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}