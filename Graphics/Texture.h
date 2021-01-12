#pragma once

namespace Graphics
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(const std::string filePath);
		~Texture2D();

		void Bind(uint32_t slot) const;

		operator GLuint() const { return m_TextureID; }
	private:
		GLuint m_TextureID;
	};

	class TextureLibrary
	{
	public:
		static void Init();

		static const std::shared_ptr<Texture2D>& LoadTexture(const std::string filePath);

		static  const std::shared_ptr<Texture2D>& DefaultTexture()
		{
			return s_Instance->m_DefaultTexture;
		}

		TextureLibrary(const TextureLibrary&) = delete;
		TextureLibrary& operator=(const TextureLibrary&) = delete;
	private:
		TextureLibrary() = default;
	private:
		std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_TextureCache;
		std::shared_ptr<Texture2D> m_DefaultTexture;

		static TextureLibrary* s_Instance;
	};
}