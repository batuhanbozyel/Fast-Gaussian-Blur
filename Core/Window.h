#pragma once
#include <GLFW/glfw3.h>
#include "WindowEvent.h"

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

		GLFWwindow* GetNativeWindow() const { return m_Window; }
		const WindowProps& GetProps() const { return m_Props; }
	private:
		WindowProps m_Props;
		GLFWwindow* m_Window;
	};
}