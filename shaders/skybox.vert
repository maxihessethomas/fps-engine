#version 410

layout (location = 0) in vec3 aPos;

out vec3 viewDirection;

uniform mat4 perspective;
uniform mat4 view;

void main()
{
    viewDirection = aPos;

    vec4 position = perspective * mat4(mat3(view)) * vec4(aPos, 1.0);

    gl_Position = position.xyww;
}
