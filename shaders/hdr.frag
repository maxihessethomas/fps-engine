#version 410

in vec2 TexCoord;

uniform sampler2D hdrBuffer;
uniform float exposure;

out vec4 FragColor;

void main()
{
    const float gamma = 2.2;

    vec3 hdrColor = texture(hdrBuffer, TexCoord).rgb;

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
}
