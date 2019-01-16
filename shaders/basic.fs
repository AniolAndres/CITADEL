#version 400 core

out vec4 color;

in vec2 uv0;

uniform sampler2D diffuseMap;
uniform int drawTexture;

uniform sampler2D occlusionMap;

uniform float ambient;
uniform float k_ambient;

uniform vec4 ambientcolor;

void main()
{
	vec3 finalAmbient = k_ambient * ambient * vec3(texture(occlusionMap, uv0));

	color = texture2D(diffuseMap, uv0) + vec4(finalAmbient,1.0f);
}
