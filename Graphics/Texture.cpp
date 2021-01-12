#include "pch.h"
#include <glad/glad.h>
#include "Texture.h"

#include <stb_image/stb_image.h>

namespace Graphics
{
	TextureLibrary* TextureLibrary::s_Instance = nullptr;

	Texture2D::Texture2D()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);

		unsigned char buffer[4] = { 25, 25, 25, 255 };
		glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureStorage2D(TextureID, 1, GL_RGBA8, 1, 1);
		glTextureSubImage2D(TextureID, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}

	Texture2D::Texture2D(const std::string filePath)
	{
		stbi_set_flip_vertically_on_load(1);

		glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);

		unsigned char* buffer = stbi_load(filePath.c_str(), &Width, &Height, &Channels, 4);
		if (buffer)
		{
			glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureStorage2D(TextureID, 1, GL_RGBA8, Width, Height);
			glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
		stbi_image_free(buffer);
	}
	
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &TextureID);
	}

	void TextureLibrary::Init()
	{
		if (!s_Instance)
		{
			s_Instance = new TextureLibrary;
			s_Instance->m_DefaultTexture = std::make_shared<Texture2D>();
		}
	}

	const std::shared_ptr<Texture2D>& TextureLibrary::LoadTexture(const std::string filePath)
	{
		const auto& textureCacheIt = s_Instance->m_TextureCache.find(filePath);

		if (textureCacheIt != s_Instance->m_TextureCache.end())
			return textureCacheIt->second;

		const auto& insertedTextureIt = s_Instance->m_TextureCache.insert(
			textureCacheIt,
			std::make_pair(filePath, std::make_shared<Texture2D>(filePath))
		);

		return insertedTextureIt->second;
	}
}