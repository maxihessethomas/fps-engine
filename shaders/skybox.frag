#version 410

in vec3 viewDirection;

out vec4 FragColor;

uniform vec3 upperHemisphereColor;
uniform vec3 lowerHemisphereColor;

void main()
{
    vec3 dir = normalize(viewDirection);

    float height = dir.y;

    float interpolationFactor = smoothstep(0.0, 1.0, height);

    vec3 color = mix(lowerHemisphereColor, upperHemisphereColor, interpolationFactor);

    FragColor = vec4(color, 1.0);
}
