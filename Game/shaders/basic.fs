#version 400 core

out vec4 color;

in vec2 uv0;

uniform sampler2D diffuseMap;
uniform int drawTexture;

void main()
{

color = texture2D(diffuseMap, uv0);
}
