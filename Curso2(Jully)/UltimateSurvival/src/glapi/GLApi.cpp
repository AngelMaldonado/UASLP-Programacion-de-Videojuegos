#include <glapi/GLApi.h>
/*
 * TODO: Add to ~() other termination instances of the api
 * TODO: Pass callbacks functions to its own bind method
 */

GLApi* GLApi::_glApi;

GLApi::~GLApi()
{
    if (_glApi != nullptr)
    {
        glfwTerminate();
    }
}

GLApi* GLApi::GetInstance()
{
    if (_glApi == nullptr)
    {
		_glApi = new GLApi();
        _glApi->InitGLFWVersion();
		
        _glApi->window = Window(WindowSettings());
		_glApi->SetCurrentWindow(_glApi->window);
		_glApi->GladLoad();

        _glApi->window.InitWindow();
    }
	return _glApi;
}

void GLApi::InitGLFWVersion()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLAPI_GLFW_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLAPI_GLFW_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GLApi::GladLoad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Failed to initialize GLAD" << std::endl;
}

void GLApi::SetCurrentWindow(Window window)
{
    if (_glApi != nullptr)
    {
        _glApi->window = window;
        glfwMakeContextCurrent(_glApi->window.GetGLFWwindow());
    }
}

void GLApi::GLValidate()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLApi::GLLogCall(const char* function, const char* file, int line)
{
    const char* message;
    while (int code = glfwGetError(&message))
    {
        std::cout << "[OpenGL Error] ( " << code << " ):\n" << 
        "- Call: " << function << "\n- File: " << file << "\n- Line: " << line << "\n# Error: " << message;
        return false;
    }
    return true;
}

