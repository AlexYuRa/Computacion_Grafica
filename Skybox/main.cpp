#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "Animation.h"
#include "Animator.h"
#include "Skybox.h"

#include <iostream>

using namespace std;

// Prototipos
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Configuración inicial
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    // --- Inicialización de GLFW ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Animacion 3D - Blender + OpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Error: No se pudo crear la ventana GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- Inicializar GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Error al inicializar GLAD" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // --- Shaders con soporte para huesos ---
    Shader shader("shaders/model_loading.vert", "shaders/model_loading.frag");

    // --- Shader para skybox ---
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    // --- Cargar modelo con huesos (GLB o FBX exportado desde Blender) ---
    Model ourModel("assets/robot.glb");

    // --- Cargar animación y crear animador ---
    Animation animation("assets/robot.glb", &ourModel);
    Animator animator(&animation);

    // --- Configurar skybox ---
    vector<string> faces = {
        "assets/skybox_textures/right.jpg",
        "assets/skybox_textures/left.jpg",
        "assets/skybox_textures/top.jpg",
        "assets/skybox_textures/bottom.jpg",
        "assets/skybox_textures/front.jpg",
        "assets/skybox_textures/back.jpg"
    };
    Skybox skybox(faces);

    // --- Loop principal ---
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Matrices de cámara ---
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, -5.0f));

        // --- Renderizar skybox primero (como fondo) ---
        skybox.Draw(skyboxShader, view, projection);

        // --- Renderizar modelo animado ---
        shader.use();

        // Actualizar animación
        animator.UpdateAnimation(deltaTime);

        // Pasar matrices de huesos al shader
        auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Orientar desde Blender
        shader.setMat4("model", model);

        // Dibujar modelo animado
        ourModel.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// --- Callbacks y entradas ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
