#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 lightPos = vec3(0.0, 3.0, 3.0);
    vec3 lightColor = vec3(1.0);
    vec3 objectColor = texture(texture_diffuse1, TexCoords).rgb;

    // Iluminación Phong básica
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = diffuse * objectColor;
    FragColor = vec4(result, 1.0);
}
