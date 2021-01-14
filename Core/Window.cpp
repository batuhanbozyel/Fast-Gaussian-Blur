#include "pch.h"
#include "Window.h"
#include <glad/glad.h>

namespace Core
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		Log::Error("Window Error: {0}", description);
	}

	static void OpenGLMessageCallback(
		unsigned int source,
		unsigned int type,
		unsigned int id,
		unsigned int severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			Core::Log::Critical(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:			Core::Log::Error(message); return;
			case GL_DEBUG_SEVERITY_LOW:				Core::Log::Warn(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	Core::Log::Trace(message); return;
		}
	}

	Window::Window(const WindowProps& props)
		: m_Props(props)
	{
		int glfw = glfwInit();
		LOG_ASSERT(glfw, "GLFW initialization failed!");

#ifdef PDEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		glfwSetErrorCallback(GLFWErrorCallback);

		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		m_Window = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);

		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		m_Props.Width = width;
		m_Props.Height = height;

		LOG_ASSERT(m_Window, "Window creation failed!");
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LOG_ASSERT(status, "Glad initialization failed!");

#ifdef PDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

		LOG_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "OpenGL 4.5 is not supported by your hardware!");

		Core::Log::Info(glGetString(GL_RENDERER));
		Core::Log::Info(glGetString(GL_VERSION));

		glfwSetWindowUserPointer(m_Window, &m_Props);
		glfwSwapInterval(m_Props.VSync);

		// WindowResize Event
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
				WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(window);
				props.Width = static_cast<uint32_t>(width);
				props.Height = static_cast<uint32_t>(height);

				props.EventCallback(WindowResizeEvent(props.Width, props.Height));
		});

		// WindowClose Event
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(window);

			props.EventCallback(WindowCloseEvent());
		});
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::OnWindowResize(WindowResizeEvent& e)
	{
		glfwSetWindowSize(m_Window, e.Width, e.Height);
		m_Props.Width = e.Width;
		m_Props.Height = e.Height;
	}
}