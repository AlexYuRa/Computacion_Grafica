#ifndef BONE_H
#define BONE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>

struct KeyPosition {
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation {
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale {
    glm::vec3 scale;
    float timeStamp;
};

class Bone {
public:
    Bone(const std::string& name, int ID, const aiNodeAnim* channel)
        : m_Name(name), m_ID(ID), m_LocalTransform(1.0f)
    {
        for (unsigned int i = 0; i < channel->mNumPositionKeys; i++) {
            KeyPosition data;
            data.position = glm::vec3(channel->mPositionKeys[i].mValue.x,
                                      channel->mPositionKeys[i].mValue.y,
                                      channel->mPositionKeys[i].mValue.z);
            data.timeStamp = channel->mPositionKeys[i].mTime;
            m_Positions.push_back(data);
        }

        for (unsigned int i = 0; i < channel->mNumRotationKeys; i++) {
            KeyRotation data;
            data.orientation = glm::quat(channel->mRotationKeys[i].mValue.w,
                                         channel->mRotationKeys[i].mValue.x,
                                         channel->mRotationKeys[i].mValue.y,
                                         channel->mRotationKeys[i].mValue.z);
            data.timeStamp = channel->mRotationKeys[i].mTime;
            m_Rotations.push_back(data);
        }

        for (unsigned int i = 0; i < channel->mNumScalingKeys; i++) {
            KeyScale data;
            data.scale = glm::vec3(channel->mScalingKeys[i].mValue.x,
                                   channel->mScalingKeys[i].mValue.y,
                                   channel->mScalingKeys[i].mValue.z);
            data.timeStamp = channel->mScalingKeys[i].mTime;
            m_Scales.push_back(data);
        }
    }

    void Update(float animationTime) {
        glm::mat4 translation = InterpolatePosition(animationTime);
        glm::mat4 rotation = InterpolateRotation(animationTime);
        glm::mat4 scale = InterpolateScaling(animationTime);
        m_LocalTransform = translation * rotation * scale;
    }

    glm::mat4 GetLocalTransform() const { return m_LocalTransform; }
    std::string GetBoneName() const { return m_Name; }
    int GetBoneID() const { return m_ID; }

private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScaling(float animationTime);
};
#endif
