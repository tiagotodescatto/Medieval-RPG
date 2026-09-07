#version 460 core

in vec3 Normal;

out vec4 fragColor;

uniform vec3 lightDir;

void main(){
	vec3 norm = normalize(Normal);
	vec3 light = normalize(lightDir);

	float diffuseLight = max(dot(norm, light), 0.0);
	float envirommentLight = 0.2;
	float finalLight = diffuseLight + envirommentLight;
	vec3 objectColor = vec3(1.0, 0.137, 0.235);
	vec3 finalColor = objectColor * finalLight;

	fragColor = vec4(finalColor, 1.0);
}