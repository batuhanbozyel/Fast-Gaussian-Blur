#pragma once

namespace Graphics
{
	class Texture2D;
	class Shader;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static GLuint DrawTextureFiltered(const Texture2D* texture, const Shader* filterPass);
		static void ResizeTextureFramebuffer(uint32_t width, uint32_t height);
	};
}