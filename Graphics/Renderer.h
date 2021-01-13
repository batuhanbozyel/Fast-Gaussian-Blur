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
		static void BeginFilterPass(const Texture2D* texture);
		static void DrawTextureFiltered(uint32_t count);
		static GLuint EndFilterPass();
	private:
		static void RenderTexturePass(GLuint inputTexture, GLuint outputTexture, Shader* filterPass);
		static void ResizeFramebuffer(uint32_t width, uint32_t height);
		static void ConstructFramebuffer();
	};
}