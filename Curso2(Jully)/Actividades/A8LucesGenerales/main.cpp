#include <libraries.h>

#include <Camera.h>
#include "Variables.h"
#include "Cubo.h"

#include <Shader.h>
#include "Texture.h"

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLightShader, Texture ourTexture);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void GeneracionBufferCube();
void GeneracionBufferLight();
void VertexAttribute(int layout, int data, int total, int start); 
void TransformacionObjeto(Shader ourShader);
void TransformacionLuz(Shader ourLightShader);
void CameraTransform(Shader ourShader);
void UniformCamera(Shader ourShader);

int main()
{
	initGLFWVersion();

	GLFWwindow* window = glfwCreateWindow(w, h, "Luz", NULL, NULL);
	
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoad()) return -1;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertexShader.vs", "fragmenShader.fs");
	Shader ourLightShader("vertexLight.vl", "fragmenLight.fl");
	Texture ourTexture;

	GeneracionBufferCube();
	GeneracionBufferLight();

	diffuseMap = ourTexture.loadTextureID(nombre[0], expan[0]);
	specularMap = ourTexture.loadTextureID(nombre[1], expan[1]);

	ourShader.use();
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);

	updateWindow(window, ourShader, ourLightShader, ourTexture);

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
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
}

bool gladLoad()
{
	bool active = true;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		active = false;
	}
	return active;
}

void updateWindow(GLFWwindow* window, Shader ourShader, Shader ourLightShader, Texture ourTexture)
{
	while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourTexture.ViewTextureMap(0, diffuseMap);
        ourTexture.ViewTextureMap(1, specularMap);

        ourShader.use();

        //Direccional light
        ///*
        ourShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        ourShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("dirLight.direction", lightDirect);
        //*/

        //Point light
        for (int i = 0; i < totalP; ++i)
        {
            string i_str = to_string(i);
            ourShader.setVec3("pointLights[" + i_str + "].ambient", 0.05f, 0.05f, 0.05f);
            ourShader.setVec3("pointLights[" + i_str + "].diffuse", 0.8f, 0.8f, 0.8f);
            ourShader.setVec3("pointLights[" + i_str + "].specular", 1.0f, 1.0f, 1.0f);
            ourShader.setVec3("pointLights[" + i_str + "].pose", pointLightPositions[i]);
            ourShader.setFloat("pointLights[" + i_str + "].constant", constantV);
            ourShader.setFloat("pointLights[" + i_str + "].linear", linearV);
            ourShader.setFloat("pointLights[" + i_str + "].quadratic", quadraticV);         
        }       

        //Spot light
        ///*
        ourShader.setVec3("spotLight.pose", camera.Position);
        ourShader.setVec3("spotLight.direction", camera.Front);
        ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", constantV);
        ourShader.setFloat("spotLight.linear", linearV);
        ourShader.setFloat("spotLight.quadratic", quadraticV);
        ourShader.setFloat("spotLight.cutOff", cos(radians(cutOffValue)));
        ourShader.setFloat("spotLight.outerCutOff", cos(radians(outerCutOffValue)));
        //*/
        ourShader.setVec3("viewPos", camera.Position);

        ourShader.setFloat("material.shininess", 32.0f);

        CameraTransform(ourShader);

        glBindVertexArray(VAO);
        TransformacionObjeto(ourShader);

        ourLightShader.use();
        UniformCamera(ourLightShader);
        glBindVertexArray(lightVAO);
        TransformacionLuz(ourLightShader);

        if (poly)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void GeneracionBufferCube()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	VertexAttribute(0, 3, 8, 0);
	VertexAttribute(1, 3, 8, 3);
	VertexAttribute(2, 2, 8, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GeneracionBufferLight()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	VertexAttribute(0, 3, 8, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexAttribute(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}

void CameraTransform(Shader ourShader)
{
	projection = perspective(radians(camera.Zoom), (float)w / (float)h, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	UniformCamera(ourShader);
}

void UniformCamera(Shader ourShader)
{
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);
}

void TransformacionObjeto(Shader ourShader)
{
	bool one = false;
	bool rotar = true;
	int n = 10;

	if (one)
	{
		model = mat4(1.0f);
		if (rotar)
			model = rotate(model, (float)glfwGetTime() * radians(50.0f), vec3(0.5f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	else
		for (unsigned int i = 0; i < n; i++)
		{
			model = mat4(1.0f);
			model = translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (rotar)
				model = rotate(model, radians(angle), vec3(0.5f, 1.0f, 0.0f));
			ourShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
}

void TransformacionLuz(Shader ourLightShader)
{
	for (int i = 0; i < totalP; i++)
	{
		model = mat4(1.0f);
		model = translate(model, pointLightPositions[i]);
		model = scale(model, vec3(0.1f));
		ourLightShader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

