#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    mat4 boneTransform = mat4(0.0);
    for (int i = 0; i < 4; i++)
        boneTransform += finalBonesMatrices[aBoneIDs[i]] * aWeights[i];

    vec4 localPosition = boneTransform * vec4(aPos, 1.0);
    FragPos = vec3(model * localPosition);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * model * localPosition;
}
