#pragma once

namespace Graphics
{
	struct Texture2D;
	class Shader;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static GLuint DrawTextureFiltered(const Texture2D* texture, const Shader* filterPass);
	};
}