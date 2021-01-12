#include "pch.h"
#include "Editor.h"
#include <commdlg.h>
#include <imgui/imgui.h>

#include "Core/Application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <glad/glad.h>
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

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

layout(location = 0) in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
})"sv;

namespace PEditor
{
	Editor* Editor::s_Instance = nullptr;

	struct SceneData
	{
		std::shared_ptr<Graphics::Shader> GaussianBlur;
		std::shared_ptr<Graphics::Texture2D> Texture;
	} static s_Data;

	void Editor::OnStart()
	{
		s_Data.GaussianBlur = Graphics::ShaderLibrary::LoadShader(
			"GaussianBlur",
			std::string(GaussianBlurVertexShader),
			std::string(GaussianBlurFragmentShader));
		
		s_Data.Texture = Graphics::TextureLibrary::DefaultTexture();
	}

	void Editor::OnUpdate()
	{
		ImGuiBegin();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Import Texture"))
				{
					std::string file = OpenFileDialog();
					if (!file.empty())
					{
						Core::Log::Warn("Opened Texture File: {0}", file);
						s_Data.Texture = Graphics::TextureLibrary::LoadTexture(file);
					}
				}

				if (ImGui::MenuItem("Exit"))
					Core::Application::OnWindowClose(Core::WindowCloseEvent());

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		GLuint blurredImage = Graphics::Renderer::DrawTextureFiltered(s_Data.Texture.get(), s_Data.GaussianBlur.get());

		static bool dockspaceOpen = true;
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Image", &dockspaceOpen, ImGuiWindowFlags_NoBackground);
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			ImGui::Image(reinterpret_cast<void*>(s_Data.Texture->operator GLuint()), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}
		
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Blurred", &dockspaceOpen, ImGuiWindowFlags_NoBackground);
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			ImGui::Image(reinterpret_cast<void*>(blurredImage), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
			Graphics::Renderer::ResizeTextureFramebuffer(viewportSize.x, viewportSize.y);
			ImGui::End();
			ImGui::PopStyleVar();
		}
		
		ImGui::Begin("Renderer Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SameLine();
		ImGui::End();
	
		ImGuiEnd();
	}

	void Editor::ImGuiBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}

	void Editor::ImGuiEnd()
	{
		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(
			(float)Core::Application::GetWindow().GetProps().Width,
			(float)Core::Application::GetWindow().GetProps().Height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	Editor::Editor()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsLight();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(Core::Application::GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	std::string Editor::OpenFileDialog()
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window(Core::Application::GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;

		ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		std::string fileName;
		if (GetOpenFileNameA(&ofn))
			fileName = ofn.lpstrFile;

		return fileName;
	}

	Editor::~Editor()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	std::unique_ptr<Editor> Editor::Create()
	{
		if (!s_Instance)
			s_Instance = new Editor;

		return std::unique_ptr<Editor>(s_Instance);
	}
}