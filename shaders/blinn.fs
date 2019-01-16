#version 400 core

out vec4 color;

uniform sampler2D diffuseMap;
uniform float k_diffuse;

uniform sampler2D specularMap;
uniform vec4 specularColor;
uniform float k_specular;

uniform int drawTexture;

uniform sampler2D occlusionMap;

uniform float ambient;
uniform float k_ambient;

uniform float shininess;

uniform vec3 light_pos;

uniform vec4 ambientcolor;

uniform mat4 proj; 
uniform mat4 view; 
uniform mat4 model;

in vec2 uv0;
in vec3 position;
in vec3 normal; 

void main()
{
	vec3 finalAmbient = k_ambient * ambient * vec3(texture(occlusionMap, uv0));

	vec3 normal = normalize(normal);
	vec3 lightDir = normalize(light_pos - position);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 finalDiffuse = k_diffuse * diff * vec3(texture(diffuseMap, uv0));

    vec3 viewPos = transpose(mat3(view))*(-view[3].xyz);
	vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec4 textureSpecularColor = texture(specularMap, uv0);
	vec4 algo = vec4(textureSpecularColor.rgb * specularColor.rgb, max(textureSpecularColor.a * shininess * 128.0f, 8.0f));
	vec3 finalSpecular = algo.rgb * k_specular * spec * specularColor.rgb;

	color = vec4(finalDiffuse,1.0f) + vec4(finalAmbient,1.0f) + vec4(finalSpecular, 1.0f);
}
