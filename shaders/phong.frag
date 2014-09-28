#version 300 es

in highp vec3 LightIntensity;

layout( location = 0 ) out highp vec4 FragColor;

void main() {
    FragColor = vec4(LightIntensity, 1.0);
}
