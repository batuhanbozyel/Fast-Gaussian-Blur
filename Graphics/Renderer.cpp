#include "pch.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

namespace Graphics
{
	struct Framebuffer
	{
		GLuint RendererID;
		GLuint ColorAttachment;
		uint32_t Width = 1280, Height = 720;
	};

	struct RendererData
	{
		Framebuffer RenderTarget;
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
		
		glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.RenderTarget.ColorAttachment);
		glTextureParameteri(s_Data.RenderTarget.ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(s_Data.RenderTarget.ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(s_Data.RenderTarget.ColorAttachment, 1, GL_RGBA8, s_Data.RenderTarget.Width, s_Data.RenderTarget.Height);

		glCreateFramebuffers(1, &s_Data.RenderTarget.RendererID);
		glNamedFramebufferTexture(s_Data.RenderTarget.RendererID, GL_COLOR_ATTACHMENT0, s_Data.RenderTarget.ColorAttachment, 0);

		LOG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer does not meet the requirements!");
	}

	void Renderer::Shutdown()
	{
		glDeleteVertexArrays(1, &s_Data.VertexArray);

		glDeleteFramebuffers(1, &s_Data.RenderTarget.RendererID);
		glDeleteTextures(1, &s_Data.RenderTarget.ColorAttachment);
	}

	GLuint Renderer::DrawTextureFiltered(const Texture2D* texture, const Shader* filterPass)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.RenderTarget.RendererID);
		glViewport(0, 0, s_Data.RenderTarget.Width, s_Data.RenderTarget.Height);
		//glClear(GL_COLOR_BUFFER_BIT);

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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return s_Data.RenderTarget.ColorAttachment;
	}

	void Renderer::ResizeTextureFramebuffer(uint32_t width, uint32_t height)
	{
		if (s_Data.RenderTarget.Width != width && s_Data.RenderTarget.Height != height)
		{
			s_Data.RenderTarget.Width = width;
			s_Data.RenderTarget.Height = height;

			glDeleteFramebuffers(1, &s_Data.RenderTarget.RendererID);
			glDeleteTextures(1, &s_Data.RenderTarget.ColorAttachment);

			glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.RenderTarget.ColorAttachment);
			glTextureParameteri(s_Data.RenderTarget.ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(s_Data.RenderTarget.ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureStorage2D(s_Data.RenderTarget.ColorAttachment, 1, GL_RGBA8, s_Data.RenderTarget.Width, s_Data.RenderTarget.Height);

			glCreateFramebuffers(1, &s_Data.RenderTarget.RendererID);
			glNamedFramebufferTexture(s_Data.RenderTarget.RendererID, GL_COLOR_ATTACHMENT0, s_Data.RenderTarget.ColorAttachment, 0);

			LOG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer does not meet the requirements!");

			Core::Log::Trace("Framebuffer Resized to : {0}, {1}", width, height);
		}
	}
}