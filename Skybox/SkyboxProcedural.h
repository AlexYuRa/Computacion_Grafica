// Ejemplo de cómo generar texturas de skybox proceduralmente para testing
// Esto es solo para desarrollo - en producción usarías imágenes reales

#ifndef SKYBOX_PROCEDURAL_H
#define SKYBOX_PROCEDURAL_H

#include <glad/glad.h>
#include <vector>

// Función para crear una textura de color sólido como placeholder
unsigned int createColorTexture(float r, float g, float b, int width = 512, int height = 512)
{
    std::vector<unsigned char> data(width * height * 3);
    
    for (int i = 0; i < width * height; i++)
    {
        data[i * 3 + 0] = (unsigned char)(r * 255); // R
        data[i * 3 + 1] = (unsigned char)(g * 255); // G  
        data[i * 3 + 2] = (unsigned char)(b * 255); // B
    }
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    return texture;
}

// Función para crear un cubemap con colores diferentes para cada cara
unsigned int createProceduralCubemap()
{
    unsigned int cubemap;
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    
    // Colores para cada cara del cubo
    float colors[6][3] = {
        {1.0f, 0.0f, 0.0f}, // Right - Rojo
        {0.0f, 1.0f, 0.0f}, // Left - Verde  
        {0.0f, 0.0f, 1.0f}, // Top - Azul
        {1.0f, 1.0f, 0.0f}, // Bottom - Amarillo
        {1.0f, 0.0f, 1.0f}, // Front - Magenta
        {0.0f, 1.0f, 1.0f}  // Back - Cian
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
    
    return cubemap;
}

#endif