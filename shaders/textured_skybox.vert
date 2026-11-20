#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 perspective;
uniform mat4 view;

void main()
{
    TexCoords = vec3(aPos.x, aPos.y, aPos.z);
    vec4 pos = perspective * mat4(mat3(view)) * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
}
