#include "pch.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

namespace Graphics
{
	struct RendererData
	{
		GLuint VertexArray;
		const Shader* ActiveShader = nullptr;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		ShaderLibrary::Init();
		TextureLibrary::Init();

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glCreateVertexArrays(1, &s_Data.VertexArray);
		glBindVertexArray(s_Data.VertexArray);
	}

	void Renderer::Shutdown()
	{
		glDeleteVertexArrays(1, &s_Data.VertexArray);
	}

	void Renderer::DrawTextureFiltered(const Texture2D* texture, const Shader* filterPass)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if (!s_Data.ActiveShader || (*s_Data.ActiveShader != *filterPass))
		{
			s_Data.ActiveShader = filterPass;
			filterPass->Bind();
		}

		texture->Bind(0);

		struct DrawArraysIndirectCommand
		{
			GLuint Count;
			GLuint PrimCount;
			GLuint First;
			GLuint BaseInstance;

			DrawArraysIndirectCommand(GLuint count, GLuint primCount, GLuint first, GLuint baseInstance)
				: Count(count), PrimCount(primCount), First(first), BaseInstance(baseInstance)
			{

			}
		}static const indirect (6, 2, 0, 0);

		glDrawArraysIndirect(GL_TRIANGLES, &indirect);
	}
}

