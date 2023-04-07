#include "libraries.h"

#include "Camera.h"
#include "Variables.h"
#include "Cubo.h"

#include "Shader.h"
#include "Texture.h"

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader, Texture ourTexture);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void GeneracionBuffer();
void VertexAttribute(int layout, int data, int total, int start);

void TransformacionObjeto(Shader ourShader);
void CameraTransform(Shader ourShader);

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

	if (!gladLoad()) return -1;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	limite = (sizeof(texture) / sizeof(texture[0]));

	Shader ourShader("vertexShader.vs", "fragmenShader.fs");
	Texture ourTexture(texture, limite);

	GeneracionBuffer();

	for (int i = 0; i < limite; i++)
		ourTexture.GeneraTextura(texture[i], nombre[i], tipo[i], expan[i]);

	if (limite > 1)
	{
		ourShader.use();
		for (int i = 0; i < limite; i++)
			ourShader.setInt(ourTexture.UniformTexture(), i);
	}

	updateWindow(window, ourShader, ourTexture);

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

void updateWindow(GLFWwindow* window, Shader ourShader, Texture ourTexture)
{
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourTexture.ViewTexture();
		ourShader.use();

		CameraTransform(ourShader);
		
		glBindVertexArray(VAO);
		TransformacionObjeto(ourShader);

		if (poly) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

	VertexAttribute(0, 3, 5, 0);
	VertexAttribute(1, 2, 5, 3);

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
	mat4 projection = perspective(radians(camera.Zoom), (float)w / (float)h, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
}

void TransformacionObjeto(Shader ourShader)
{
	bool one = true;
	int n = 10;

	if (one)
	{
		mat4 model = mat4(1.0f);
		model = rotate(model, (float)glfwGetTime() * radians(50.0f), vec3(0.5f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	else
		for (unsigned int i = 0; i < n; i++)
		{
			mat4 model = mat4(1.0f);
			model = translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = rotate(model, radians(angle), vec3(0.5f, 1.0f, 0.0f));
			ourShader.setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
}
