#version 410

in vec3 viewDirection;

out vec4 FragColor;

uniform vec3 upperHemisphereColor;
uniform vec3 lowerHemisphereColor;

void main()
{
    vec3 dir = normalize(viewDirection);

    float height = dir.y;

    float interpolationFactor = dir.y * 0.5 + 0.5;

    vec3 color = mix(lowerHemisphereColor, upperHemisphereColor, interpolationFactor);

    FragColor = vec4(color, 1.0);
}
