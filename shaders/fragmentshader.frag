#version 460 core

in vec3 Normal;
in vec3 LocalPos;

out vec4 fragColor;

uniform vec3 lightDir;
uniform samplerCube skybox;

void main(){
    vec3 norm = normalize(Normal);
    vec3 light = normalize(lightDir);

    float diff = max(dot(norm, light), 0.0);
    float ambient = 0.15;
    vec3 objColor = vec3(1.0, 1.0, 1.0);

    vec3 finalColor = objColor * (diff + ambient);

    fragColor = vec4(finalColor, 1.0);
}