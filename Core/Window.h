#pragma once
#include <GLFW/glfw3.h>
#include "WindowEvent.h"
#include "Graphics/Context.h"

namespace Core
{
	struct WindowProps
	{
		std::function<void(Event&)> EventCallback;
		uint32_t Width = 1280, Height = 720;
		std::string Title = "Gaussian Blur";
		bool VSync = false;
	};

	class Window
	{
	public:
		Window(const WindowProps& props);
		~Window();

		void OnUpdate();
		void OnWindowResize(WindowResizeEvent& e);
	private:
		WindowProps m_Props;
		GLFWwindow* m_Window;
		std::unique_ptr<Graphics::Context> m_Context;
	private:
		static uint8_t s_WindowCount;
	};
}