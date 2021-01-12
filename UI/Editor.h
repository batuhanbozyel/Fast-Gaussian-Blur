#pragma once

namespace PEditor
{
	class Editor
	{
	public:
		~Editor();

		static std::unique_ptr<Editor> Create();

		void OnStart();
		void OnUpdate();

		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
	private:
		Editor();

		static std::string OpenFileDialog();

		static void ImGuiBegin();
		static void ImGuiEnd();
	private:
		static Editor* s_Instance;
	};
}
