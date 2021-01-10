#pragma once
#include <GLFW/glfw3.h>

namespace Graphics
{
	class Context
	{
	public:
		Context(GLFWwindow* window);
		~Context() = default;
	};
}