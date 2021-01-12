#include "pch.h"
#include <glad/glad.h>
#include "Texture.h"

#include <stb_image/stb_image.h>

namespace Graphics
{
	TextureLibrary* TextureLibrary::s_Instance = nullptr;

	Texture2D::Texture2D()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

		unsigned char buffer[4] = { 25, 25, 25, 255 };
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureStorage2D(m_TextureID, 1, GL_RGBA8, 1, 1);
		glTextureSubImage2D(m_TextureID, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	}

	Texture2D::Texture2D(const std::string filePath)
	{
		stbi_set_flip_vertically_on_load(1);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

		int width, height, channels;
		unsigned char* buffer = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
		if (buffer)
		{
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureStorage2D(m_TextureID, 1, GL_RGBA8, width, height);
			glTextureSubImage2D(m_TextureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
		stbi_image_free(buffer);
	}
	
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
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