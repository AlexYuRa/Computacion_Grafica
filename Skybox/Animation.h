#ifndef ANIMATION_H
#define ANIMATION_H

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>

#include "Bone.h"
#include "Model.h"  // para obtener la jerarquía de nodos (NodeData)

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    Animation() = default;

    Animation(const std::string& animationPath, Model* model)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights);

        if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        {
            std::cout << "Error al cargar animación: " << importer.GetErrorString() << std::endl;
            return;
        }

        auto animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f;

        ReadHierarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBones(animation, *model);
    }

    Bone* FindBone(const std::string& name)
    {
        auto iter = m_Bones.find(name);
        if (iter == m_Bones.end())
            return nullptr;
        else
            return &iter->second;
    }

    float GetTicksPerSecond() const { return m_TicksPerSecond; }
    float GetDuration() const { return m_Duration; }
    const AssimpNodeData& GetRootNode() const { return m_RootNode; }
    const std::map<std::string, BoneInfo>& GetBoneIDMap() const { return m_BoneInfoMap; }

private:
    float m_Duration;
    float m_TicksPerSecond;
    std::map<std::string, Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;

    void ReadMissingBones(const aiAnimation* animation, Model& model)
    {
        int size = animation->mNumChannels;

        auto& boneInfoMap = model.GetBoneInfoMap(); // referencia al mapa del modelo
        int& boneCount = model.GetBoneCount();

        for (int i = 0; i < size; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;

            if (boneInfoMap.find(boneName) == boneInfoMap.end())
            {
                BoneInfo newBoneInfo;
                newBoneInfo.id = boneCount;
                newBoneInfo.offset = glm::mat4(1.0f);
                boneInfoMap[boneName] = newBoneInfo;
                boneCount++;
            }
            m_Bones[boneName] = Bone(boneName, boneInfoMap[boneName].id, channel);
        }

        m_BoneInfoMap = boneInfoMap;
    }

    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
    {
        dest.name = src->mName.data;
        dest.transformation = AssimpToGlmMatrix(src->mTransformation);

        for (unsigned int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newChild;
            ReadHierarchyData(newChild, src->mChildren[i]);
            dest.children.push_back(newChild);
        }
    }

    glm::mat4 AssimpToGlmMatrix(const aiMatrix4x4& from)
    {
        glm::mat4 to;
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }
};

#endif
