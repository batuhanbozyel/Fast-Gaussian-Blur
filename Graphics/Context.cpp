#include "pch.h"
#include "Context.h"
#include <glad/glad.h>

namespace Graphics
{
	static void OpenGLMessageCallback(
		unsigned int source,
		unsigned int type,
		unsigned int id,
		unsigned int severity,
		int length,
		const char* message,
		const void* userParam)
	{
#ifdef PDEBUG
		__debugbreak();
#endif
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			Core::Log::Critical(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:			Core::Log::Error(message); return;
			case GL_DEBUG_SEVERITY_LOW:				Core::Log::Warn(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	Core::Log::Trace(message); return;
		}
	}

	Context::Context(GLFWwindow* window)
	{
		LOG_ASSERT(window, "Window is nullptr!");

		glfwMakeContextCurrent(window);

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
	}
}