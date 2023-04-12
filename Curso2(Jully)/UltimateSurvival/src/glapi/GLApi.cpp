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

GLApi* GLApi::GetInstance(Window* window)
{
    if (_glApi == nullptr)
    {
		_glApi = new GLApi();
        _glApi->InitGLFWVersion();
		
        _glApi->window = window == NULL ? Window(GLAPI_DEF_WINDOW_WIDTH, GLAPI_DEF_WINDOW_HEIGHT, "GLApi") : *window;
		_glApi->SetCurrentWindow(_glApi->window);
		_glApi->GladLoad();
		
		glfwSetFramebufferSizeCallback(_glApi->window.GetGLFWwindow(), _glApi->window.windowSizeFunction);
		glfwSetCursorPosCallback(_glApi->window.GetGLFWwindow(), _glApi->window.cursorPositionFunction);

		glfwSetScrollCallback(_glApi->window.GetGLFWwindow(), _glApi->window.scrollFunction);
		glfwSetInputMode(_glApi->window.GetGLFWwindow(), _glApi->window.inputMode, _glApi->window.cursorMode);
		glfwSetKeyCallback(_glApi->window.GetGLFWwindow(), _glApi->window.keyFunction);

		if (_glApi->window.mode3D) GLCall(glEnable(GL_DEPTH_TEST));
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
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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

