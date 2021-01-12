#include "pch.h"
#include "Window.h"

namespace Core
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		Log::Error("Window Error: {0}", description);
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

		m_Window = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);
		LOG_ASSERT(m_Window, "Window creation failed!");
		m_Context = std::make_unique<Graphics::Context>(m_Window);

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