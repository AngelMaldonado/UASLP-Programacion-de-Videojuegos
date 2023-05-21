#include <game/Game.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

WindowSettings window;

Game::Game() { 
	window.name = "hola";
	window.width = 600;
	window.height = 600;
	window.windowSizeFunction = framebuffer_size_callback;
	glApi = GLApi::GetInstance();
}

Game::~Game()
{
	glApi->~GLApi();
}

void Game::Run()
{
	glApi->Init(window);

	Shader shader("vertexShader.vs", "fragmenShader.fs");

	VertexBufferLayout bufferLayout;
	VertexArrayObject vao = VertexArrayObject(true);
	VertexBufferObject vbo = VertexBufferObject(CUBE_BUFFER, CUBE_BUFFER_SIZE);
	ElementBufferObject ebo = ElementBufferObject(CUBE_INDICES, CUBE_NUM_INDICES);

	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(2);
	vao.AddBuffer(vbo, bufferLayout);

	glApi->window.renderer.SetBufferObjects(vao, vbo, ebo);

	while (glApi->window.WindowIsOpen())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        /**
        * Limpieza del buffer de profundiad
        */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)window.width / (float)window.height, 0.1f, 100.0f);

        shader.SetUniform4matf("model", model);
        shader.SetUniform4matf("view", view);
        shader.SetUniform4matf("projection", projection);
		glApi->window.renderer.Render();
		glfwSwapBuffers(glApi->window.GetGLFWwindow());
		glfwPollEvents();
	}
}

