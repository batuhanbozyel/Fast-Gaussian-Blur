#include "pch.h"
#include "Application.h"

#include <glad/glad.h>
#include "Graphics/Renderer.h"

int main()
{
	Core::Application::Init();

	Core::Application::Run();

	Core::Application::Shutdown();

	return 0;
}

void Core::Application::Init()
{
	Core::Log::Init();

	Core::WindowProps props;
	props.EventCallback = BIND_EVENT_FN(Core::Application::OnEvent);
	s_Window = std::make_unique<Core::Window>(props);
	s_Editor = PEditor::Editor::Create();

	Graphics::Renderer::Init();
}

void Core::Application::Shutdown()
{
	Graphics::Renderer::Shutdown();
	s_Window.reset();
}

void Core::Application::Run()
{
	s_Editor->OnStart();

	while (s_Running)
	{
		s_Editor->OnUpdate();

		s_Window->OnUpdate();
	}
}

bool Core::Application::OnWindowClose(Core::WindowCloseEvent& e)
{
	s_Running = false;
	return true;
}

bool Core::Application::OnWindowResize(Core::WindowResizeEvent& e)
{
	s_Window->OnWindowResize(e);
	return true;
}

void Core::Application::OnEvent(Core::Event& e)
{
	Core::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Core::WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<Core::WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
}

bool Core::Application::s_Running = true;
std::unique_ptr<Core::Window> Core::Application::s_Window = nullptr;
std::unique_ptr<PEditor::Editor> Core::Application::s_Editor = nullptr;
