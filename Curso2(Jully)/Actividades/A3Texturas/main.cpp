#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "stb_image.h"

const unsigned int w = 500;
const unsigned int h = 300;

float vertices[] = {
    0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f,1.0f,   3.0f, 3.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,   3.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,0.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f,1.0f,   0.0f, 3.0f
};

unsigned int indices[]{
    0,1,3,
    1,2,3
};

unsigned int VBO, VAO, EBO;
const int nTextures = 2;
unsigned int textures[nTextures];

string texturesFiles[] = {
    "Imagenes/caja.jpg",
    "Imagenes/fragil.png"
};

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void GeneracionBuffer();
void GeneraTextura(int tipo, unsigned int* textura, const char* archivo);

int main()
{
    initGLFWVersion();

    GLFWwindow* window = glfwCreateWindow(w, h, "Texturas", NULL, NULL);
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
    GeneraTextura(1, &textures[0], texturesFiles[0].c_str());
    GeneraTextura(0, &textures[1], texturesFiles[1].c_str());

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

	    glUniform1i(glGetUniformLocation(ourShader.ID, "texture0"), 0);
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);		
	    
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);		
        
        /*
        for (int textura = 0; textura < nTextures; textura++) {
            glUniform1i(glGetUniformLocation(ourShader.ID, "texture" + textura), 0);
			glActiveTexture(GL_TEXTURE0 + textura);
			glBindTexture(GL_TEXTURE_2D, textures[textura]);
        } */

        ourShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
       // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GeneraTextura(int tipo, unsigned int* textura, const char* archivo)
{
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int width, height, nrChannels;
    unsigned char* data;
	glGenTextures(1, textura);
	glBindTexture(GL_TEXTURE_2D, *textura);
	
    stbi_set_flip_vertically_on_load(true);
	data = stbi_load(archivo, &width, &height, &nrChannels, 0);

    if (!data) {
        cout << "No se pudo cargar la textura..." << endl;
        return;
    }

    switch (tipo) {
        // PNG
		case 0:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
        // JPG
		case 1:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
    }
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}
