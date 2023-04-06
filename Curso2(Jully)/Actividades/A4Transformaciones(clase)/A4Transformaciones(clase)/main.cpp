#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

const unsigned int width = 500;
const unsigned int height = 500;
const bool poly = false;

float vertices[] = {
    0.5f,   0.5f,   0.0f,   1.0f, 0.0f, 0.0f,   1.0f,1.0f, 1.0f, 1.0f,
    0.5f,   -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f,0.0f, 1.0f, 0.0f,
    -0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f,0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,   0.0f,   1.0f, 1.0f, 0.0f,   0.0f,1.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

unsigned int VBO, VAO, EBO;
unsigned int texture[2];
int limite;

string nombre[] = {
    "Imagenes/caja.jpg",
    "Imagenes/fragil.png"
};

int tipo[] = {
    0,
    1
};

int expan[] = {
    0,
    3
};

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow *window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void GeneracionBuffer();
void VertexAttribute(int layout, int data, int total, int start);
void GeneraTextura(int n, string path, int td, int e);

int main(void)
{
    initGLFWVersion();

    GLFWwindow *window = glfwCreateWindow(width, height, "Texturas", NULL, NULL);
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

    Shader ourShader("vertexShader.vs", "fragmenShader.fs");

    GeneracionBuffer();

    limite = (sizeof(texture) / sizeof(texture[0]));

    for (int i = 0; i < limite; i++)
        GeneraTextura(i, nombre[i], tipo[i], expan[i]);

    if (limite > 1)
    {
		ourShader.use();
		ourShader.setInt("texture1", 0);
		ourShader.setInt("texture2", 1);
    }

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
        cout << "Failed to initialize GLAD" << endl;
        return active = false;
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

        for (int i = 0; i < limite; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
        }

        mat4 transform = mat4(1.0f);
        transform = translate(transform, vec3(0.5f, 0.5f, 0.0f));
        transform = rotate(transform, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
        transform = scale(transform, vec3(0.5f, 0.5f, 0.5f));

        ourShader.use();
        ourShader.setMat4("transform", transform);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        if (poly)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        glfwSetWindowShouldClose(window, true);
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

    VertexAttribute(0, 3, 10, 0);
    VertexAttribute(1, 3, 10, 3);
    VertexAttribute(2, 2, 10, 6);
    VertexAttribute(3, 2, 10, 8);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexAttribute(int layout, int data, int total, int start)
{
    glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void *)(start * sizeof(float)));
    glEnableVertexAttribArray(layout);
}

void GeneraTextura(int n, string path, int td, int e)
{
    glGenTextures(1, &texture[n]);
    glBindTexture(GL_TEXTURE_2D, texture[n]);

    if (e == 0)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else if (e == 1)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    else if (e == 2)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else if (e == 3)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
    else
    {
        cout << "Valores de 0 a 3 en r" << endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    if (data)
    {
        if (td == 0)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "No se puede cargar la textura" << endl;
    }
    stbi_image_free(data);
}
