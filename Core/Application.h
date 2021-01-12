#pragma once
#include "Window.h"
#include "UI/Editor.h"

namespace Core
{
	struct Event;
	struct WindowCloseEvent;
	struct WindowResizeEvent;

	class PEditor::Editor;

	class Application
	{
	public:
		static void Init();
		static void Shutdown();

		static void Run();

		static bool OnWindowClose(WindowCloseEvent& e);
		static bool OnWindowResize(WindowResizeEvent& e);
		static void OnEvent(Event& e);

		static const Window& GetWindow() { return *s_Window; }
		static GLFWwindow* GetNativeWindow() { return s_Window->GetNativeWindow(); }
	private:
		static bool s_Running;
		static std::unique_ptr<Window> s_Window;
		static std::unique_ptr<PEditor::Editor> s_Editor;
	};
}