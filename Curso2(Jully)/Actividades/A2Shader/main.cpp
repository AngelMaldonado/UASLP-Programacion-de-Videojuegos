#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"

const unsigned int w = 500;
const unsigned int h = 300;

float vertices[] = {
    // Figura 1
    0.0f, 0.5f, 0.0f,       0.75f, 0.54f, 0.1f,     // 0
    -0.5f, 0.0f, 0.0f,      0.67f, 0.12f, 0.07f,    // 1
    0.0f, -0.5f, 0.0f,      0.75f, 0.54f, 0.1f,     // 2
    0.5f, 0.0f, 0.0f,       0.67f, 0.12f, 0.07f,    // 3
    // Figura 2 y 3
    0.0f, 0.4f, 0.0f,       0.75f, 0.54f, 0.1f,     // 4
    -0.2f, 0.2f, 0.0f,      0.67f, 0.12f, 0.07f,    // 5
    0.0f, 0.0f, 0.0f,       0.75f, 0.54f, 0.1f,     // 6
    0.2f, -0.2f, 0.0f,      0.67f, 0.12f, 0.07f,    // 8
    0.0f, -0.4f, 0.0f,      0.75f, 0.54f, 0.1f      // 9
};

unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
    4, 5, 6,
    6, 7, 8
};

unsigned int VBO, VAO, EBO;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void GeneracionBuffer();

int main()
{
    initGLFWVersion();

    GLFWwindow* window = glfwCreateWindow(w, h, "Shader", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoad())
    {
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    GeneracionBuffer();

    updateWindow(window, ourShader);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void initGLFWVersion()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}
bool gladLoad()
{
    bool active = true;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return active;
}
void updateWindow(GLFWwindow* window, Shader ourShader)
{
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void GeneracionBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}