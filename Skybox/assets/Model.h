#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "Shader.h"
#include "stb_image.h"

// ==============================
// CONFIGURACIÓN
// ==============================
#define MAX_BONES_PER_VERTEX 4

// ==============================
// ESTRUCTURAS DE DATOS
// ==============================
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    int BoneIDs[MAX_BONES_PER_VERTEX];
    float Weights[MAX_BONES_PER_VERTEX];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

// ==============================
// CLASE MESH
// ==============================
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : vertices(vertices), indices(indices), textures(textures)
    {
        setupMesh();
    }

    void Draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
            else if (name == "texture_specular") number = std::to_string(specularNr++);

            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int VBO, EBO;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // posiciones
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // normales
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // coordenadas UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // IDs de huesos (ivec4)
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));
        // pesos
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));

        glBindVertexArray(0);
    }
};

// ==============================
// CLASE MODEL
// ==============================
class Model {
public:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> loaded_textures;

    std::map<std::string, int> boneMapping;     // Nombre del hueso → índice
    std::vector<glm::mat4> boneOffsets;         // Matriz inversa del bind pose
    std::vector<glm::mat4> finalBoneMatrices;   // Matrices finales para el shader

    Model(const std::string &path) {
        loadModel(path);
    }

    void Draw(Shader &shader) {
        for (auto &mesh : meshes)
            mesh.Draw(shader);
    }

private:
    const aiScene *scene;
    Assimp::Importer importer;

    // ==============================
    // CARGA PRINCIPAL
    // ==============================
    void loadModel(const std::string &path) {
        scene = importer.ReadFile(path, aiProcess_Triangulate |
                                          aiProcess_FlipUVs |
                                          aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error ASSIMP: " << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);

        finalBoneMatrices.resize(boneOffsets.size(), glm::mat4(1.0f));
    }

    // ==============================
    // PROCESAMIENTO RECURSIVO DE NODOS
    // ==============================
    void processNode(aiNode *node, const aiScene *scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    // ==============================
    // PROCESAMIENTO DE UNA MALLA
    // ==============================
    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        vertices.resize(mesh->mNumVertices);

        // Rellenar posiciones, normales, UVs
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            vertices[i].Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->HasNormals())
                vertices[i].Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            else
                vertices[i].Normal = glm::vec3(0.0f);

            if (mesh->mTextureCoords[0])
                vertices[i].TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            else
                vertices[i].TexCoords = glm::vec2(0.0f);

            for (int j = 0; j < MAX_BONES_PER_VERTEX; j++) {
                vertices[i].BoneIDs[j] = -1;
                vertices[i].Weights[j] = 0.0f;
            }
        }

        // Rellenar índices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Procesar huesos
        extractBoneWeights(vertices, mesh);

        // Procesar materiales
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    // ==============================
    // CARGA DE TEXTURAS
    // ==============================
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < loaded_textures.size(); j++) {
                if (std::strcmp(loaded_textures[j].path.c_str(), str.C_Str()) == 0) {
                    textures.push_back(loaded_textures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                loaded_textures.push_back(texture);
            }
        }
        return textures;
    }

    // ==============================
    // FUNCIÓN DE CARGA DE IMAGEN
    // ==============================
    unsigned int TextureFromFile(const char *path, const std::string &directory) {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format = GL_RGB;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cerr << "Fallo al cargar textura: " << filename << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    // ==============================
    // EXTRACCIÓN DE HUESOS Y PESOS
    // ==============================
    void extractBoneWeights(std::vector<Vertex> &vertices, aiMesh *mesh) {
        for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
            std::string boneName(mesh->mBones[boneIndex]->mName.data);

            int index = 0;
            if (boneMapping.find(boneName) == boneMapping.end()) {
                index = boneMapping.size();
                boneMapping[boneName] = index;
                glm::mat4 offset = aiMatrixToGlm(mesh->mBones[boneIndex]->mOffsetMatrix);
                boneOffsets.push_back(offset);
            } else {
                index = boneMapping[boneName];
            }

            for (unsigned int weightIndex = 0; weightIndex < mesh->mBones[boneIndex]->mNumWeights; weightIndex++) {
                unsigned int vertexId = mesh->mBones[boneIndex]->mWeights[weightIndex].mVertexId;
                float weight = mesh->mBones[boneIndex]->mWeights[weightIndex].mWeight;
                setVertexBoneData(vertices[vertexId], index, weight);
            }
        }
    }

    void setVertexBoneData(Vertex &vertex, int boneID, float weight) {
        for (int i = 0; i < MAX_BONES_PER_VERTEX; i++) {
            if (vertex.BoneIDs[i] < 0) {
                vertex.BoneIDs[i] = boneID;
                vertex.Weights[i] = weight;
                return;
            }
        }
    }

    // ==============================
    // CONVERSIÓN MATRICES AI → GLM
    // ==============================
    glm::mat4 aiMatrixToGlm(const aiMatrix4x4 &from) {
        glm::mat4 to;
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
        return to;
    }
};
