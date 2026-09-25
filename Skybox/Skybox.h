#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "Shader.h"
#include "stb_image.h"

class Skybox
{
public:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    
    // Constructor que carga desde archivos
    Skybox(const std::vector<std::string>& faces)
    {
        setupSkybox();
        cubemapTexture = loadCubemap(faces);
    }
    
    // Constructor alternativo que crea un skybox procedural para testing
    Skybox(bool procedural = true)
    {
        setupSkybox();
        if (procedural) {
            cubemapTexture = createProceduralCubemap();
        }
    }
    
    // Destructor
    ~Skybox()
    {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        glDeleteTextures(1, &cubemapTexture);
    }
    
    // Renderizar el skybox
    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection)
    {
        // Remover la traslación de la matriz de vista para que el skybox esté centrado
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        
        glDepthFunc(GL_LEQUAL); // Cambiar la función de profundidad para que el skybox se dibuje detrás
        shader.use();
        shader.setMat4("view", skyboxView);
        shader.setMat4("projection", projection);
        
        // Vincular VAO del skybox
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Restaurar la función de profundidad
    }

private:
    // Configurar los vértices del cubo para el skybox
    void setupSkybox()
    {
        float skyboxVertices[] = {
            // Posiciones del cubo          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        
        glBindVertexArray(0);
    }
    
    // Cargar cubemap desde 6 imágenes
    unsigned int loadCubemap(const std::vector<std::string>& faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        bool allLoaded = true;
        
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                GLenum format;
                if (nrChannels == 1)
                    format = GL_RED;
                else if (nrChannels == 3)
                    format = GL_RGB;
                else if (nrChannels == 4)
                    format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                std::cout << "Skybox texture loaded: " << faces[i] << std::endl;
            }
            else
            {
                std::cout << "Warning: No se pudo cargar textura de skybox: " << faces[i] << std::endl;
                std::cout << "Usando skybox procedural como respaldo..." << std::endl;
                stbi_image_free(data);
                allLoaded = false;
                break;
            }
        }
        
        // Si no se pudieron cargar todas las texturas, usar procedural
        if (!allLoaded) {
            glDeleteTextures(1, &textureID);
            return createProceduralCubemap();
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
    
    // Crear un cubemap procedural para testing
    unsigned int createProceduralCubemap()
    {
        unsigned int cubemap;
        glGenTextures(1, &cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        
        // Colores para cada cara del cubo
        float colors[6][3] = {
            {0.8f, 0.4f, 0.4f}, // Right - Rojo suave
            {0.4f, 0.8f, 0.4f}, // Left - Verde suave 
            {0.4f, 0.6f, 1.0f}, // Top - Azul cielo
            {0.3f, 0.3f, 0.3f}, // Bottom - Gris oscuro
            {0.8f, 0.6f, 1.0f}, // Front - Magenta suave
            {0.4f, 0.8f, 0.8f}  // Back - Cian suave
        };
        
        int width = 512, height = 512;
        
        for (int i = 0; i < 6; i++)
        {
            std::vector<unsigned char> data(width * height * 3);
            
            for (int j = 0; j < width * height; j++)
            {
                data[j * 3 + 0] = (unsigned char)(colors[i][0] * 255);
                data[j * 3 + 1] = (unsigned char)(colors[i][1] * 255);
                data[j * 3 + 2] = (unsigned char)(colors[i][2] * 255);
            }
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 
                         width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        std::cout << "Skybox procedural creado con colores de testing" << std::endl;
        return cubemap;
    }
};

#endif