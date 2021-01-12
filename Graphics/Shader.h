#pragma once

namespace Graphics
{
	class Shader
	{
	public:
		explicit Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformInt(const char* name, int value) const;
		void SetUniformInt2(const char* name, const glm::ivec2& value) const;
		void SetUniformInt3(const char* name, const glm::ivec3& value) const;
		void SetUniformInt4(const char* name, const glm::ivec4& value) const;

		void SetUniformUInt(const char* name, uint32_t value) const;
		void SetUniformUInt2(const char* name, const glm::uvec2& value) const;
		void SetUniformUInt3(const char* name, const glm::uvec3& value) const;
		void SetUniformUInt4(const char* name, const glm::uvec4& value) const;

		void SetUniformFloat(const char* name, float value) const;
		void SetUniformFloat2(const char* name, const glm::vec2& value) const;
		void SetUniformFloat3(const char* name, const glm::vec3& value) const;
		void SetUniformFloat4(const char* name, const glm::vec4& value) const;

		void SetUniformMat3(const char* name, const glm::mat3& value) const;
		void SetUniformMat4(const char* name, const glm::mat4& value) const;

		bool operator==(const Shader& shader) const
		{
			return m_ShaderHandle == shader.m_ShaderHandle;
		}

		bool operator!=(const Shader& shader) const
		{
			return m_ShaderHandle != shader.m_ShaderHandle;
		}
	private:
		void CalculateUniformLocations();
		uint32_t GetUniformLocation(const char* name) const;
		void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		GLuint m_ShaderHandle;
		std::unordered_map<std::string, uint32_t> m_UniformCache;
	};

	class ShaderLibrary
	{
	public:
		static void Init();
		~ShaderLibrary() = default;

		static const std::shared_ptr<Shader>& LoadShader(const std::string& sourcePath);
		static const std::shared_ptr<Shader>& LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		static const std::shared_ptr<Shader>& GetShader(const std::string& name)
		{
			const auto& shaderIt = s_Instance->m_ShaderCache.find(name);
			LOG_ASSERT(shaderIt != s_Instance->m_ShaderCache.end(), "Shader does not exist!");
			return shaderIt->second;
		}

		ShaderLibrary(const ShaderLibrary&) = delete;
		ShaderLibrary& operator=(const ShaderLibrary&) = delete;
	private:
		ShaderLibrary() = default;
		static std::string ReadFile(const std::string& sourcePath);
		static std::unordered_map<uint32_t, std::string> ParseShaderSource(const std::string& source);
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderCache;

		static ShaderLibrary* s_Instance;
	};
}