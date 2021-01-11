#include "pch.h"
#include "Window.h"

#include <glad/glad.h>
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

using namespace std::string_view_literals;

static constexpr std::basic_string_view GaussianBlurVertexShader = R"(
#version 450 core
		
layout(location = 0) out vec2 v_TexCoord;

void main() 
{
    float x = float(((uint(gl_VertexID) + 2u) / 3u)%2u); 
    float y = float(((uint(gl_VertexID) + 1u) / 3u)%2u); 

    gl_Position = vec4(-1.0f + x*2.0f, -1.0f+y*2.0f, 0.0f, 1.0f);
    v_TexCoord = vec2(x, y);
})"sv;

static constexpr std::basic_string_view GaussianBlurFragmentShader = R"(
#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
})"sv;

struct ApplicationData
{
	bool Running = true;
	std::unique_ptr<Core::Window> Window;
};

static ApplicationData s_AppData;

static bool OnWindowClose(Core::WindowCloseEvent& e)
{
	s_AppData.Running = false;
	return true;
}

static bool OnWindowResize(Core::WindowResizeEvent& e)
{
	s_AppData.Window->OnWindowResize(e);
	return true;
}

static void OnEvent(Core::Event& e)
{
	Core::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Core::WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<Core::WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
}

int main()
{
	Core::Log::Init();
	{
		Core::WindowProps props;
		props.EventCallback = BIND_EVENT_FN(OnEvent);
		s_AppData.Window = std::make_unique<Core::Window>(props);
	}
	Graphics::Renderer::Init();

	const std::shared_ptr<Graphics::Shader>& GaussianBlur = Graphics::ShaderLibrary::LoadShader(
		"GaussianBlur",
		std::string(GaussianBlurVertexShader),
		std::string(GaussianBlurFragmentShader));

	const std::shared_ptr<Graphics::Texture2D>& T_Logo1 = Graphics::TextureLibrary::LoadTexture(
		"res/t_logo1.jpg");

	const std::shared_ptr<Graphics::Texture2D>& T_Logo2 = Graphics::TextureLibrary::LoadTexture(
		"res/t_logo2.jpg");

	while (s_AppData.Running)
	{
		Graphics::Renderer::DrawTextureFiltered(T_Logo1.get(), GaussianBlur.get());

		s_AppData.Window->OnUpdate();
	}
	Graphics::Renderer::Shutdown();
	s_AppData.Window.reset();

	return 0;
}