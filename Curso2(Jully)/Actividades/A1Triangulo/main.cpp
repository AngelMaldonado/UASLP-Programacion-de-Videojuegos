#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std;

const int w = 500;
const int h = 300;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 tPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(tPos.x,tPos.y,tPos.z,1.0);\n"
"}\0";

const char* fragmenShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
"}\n\0";

float vertices[] = {
    // TRIANGULO 1
    -0.5f, -0.5f, 0.0f, // v1
    0.5f, -0.5f, 0.0f,  // v2
    -0.5f, 0.5f, 0.0f,  // v3
    // TRIANGULO 2
    -0.5f, 0.5f, 0.0f,  // v4
    0.5, 0.5f, 0.0f,    // v5
    0.5f, -0.5f, 0.0f   // v6
};

int s;
unsigned int VBO, VAO;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int Vertex();
int Fragment();
void CheckShader(int shader);
int UnionShaders(int v, int f);
void GeneracionBuffer();

int main()
{
    initGLFWVersion();

    GLFWwindow* window = glfwCreateWindow(w, h, "LearnOpenGL", NULL, NULL);
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

    int v = Vertex();
    int f = Fragment();
    s = UnionShaders(v, f);

    GeneracionBuffer();

    updateWindow(window);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(s);
   
    glfwTerminate();
    return 0;
}

void initGLFWVersion()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
void updateWindow(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(s);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


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

int Vertex()
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    CheckShader(vertexShader);
    return vertexShader;
}

int Fragment()
{
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmenShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckShader(fragmentShader);
    return fragmentShader;
}

void CheckShader(int shader)
{
    int success;
    char infLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infLog);
        cout << "Compilacion de shader fallo" << endl;
    }
}

int UnionShaders(int v, int f)
{
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, v);
    glAttachShader(shaderProgram, f);
    glLinkProgram(shaderProgram);

    int success;
    char infLog[512];
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infLog);
        cout << "Compilacion de union fallo" << endl;
    }

    glDeleteShader(v);
    glDeleteShader(f);

    return shaderProgram;
}

void GeneracionBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}