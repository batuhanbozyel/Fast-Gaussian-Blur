#include "pch.h"
#include <glad/glad.h>
#include "Shader.h"
#include <fstream>

namespace Graphics
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Compile(vertexSrc, fragmentSrc);
		CalculateUniformLocations();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderHandle);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ShaderHandle);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniformInt(const char* name, int value) const
	{
		glProgramUniform1i(m_ShaderHandle, GetUniformLocation(name), value);
	}

	void Shader::SetUniformInt2(const char* name, const glm::ivec2& value) const
	{
		glProgramUniform2i(m_ShaderHandle, GetUniformLocation(name), value.x, value.y);
	}

	void Shader::SetUniformInt3(const char* name, const glm::ivec3& value) const
	{
		glProgramUniform3i(m_ShaderHandle, GetUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetUniformInt4(const char* name, const glm::ivec4& value) const
	{
		glProgramUniform4i(m_ShaderHandle, GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformUInt(const char* name, uint32_t value) const
	{
		glProgramUniform1ui(m_ShaderHandle, GetUniformLocation(name), value);
	}

	void Shader::SetUniformUInt2(const char* name, const glm::uvec2& value) const
	{
		glProgramUniform2ui(m_ShaderHandle, GetUniformLocation(name), value.x, value.y);
	}

	void Shader::SetUniformUInt3(const char* name, const glm::uvec3& value) const
	{
		glProgramUniform3ui(m_ShaderHandle, GetUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetUniformUInt4(const char* name, const glm::uvec4& value) const
	{
		glProgramUniform4ui(m_ShaderHandle, GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformFloat(const char* name, float value) const
	{
		glProgramUniform1f(m_ShaderHandle, GetUniformLocation(name), value);
	}

	void Shader::SetUniformFloat2(const char* name, const glm::vec2& value) const
	{
		glProgramUniform2f(m_ShaderHandle, GetUniformLocation(name), value.x, value.y);
	}

	void Shader::SetUniformFloat3(const char* name, const glm::vec3& value) const
	{
		glProgramUniform3f(m_ShaderHandle, GetUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::SetUniformFloat4(const char* name, const glm::vec4& value) const
	{
		glProgramUniform4f(m_ShaderHandle, GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformMat3(const char* name, const glm::mat3& value) const
	{
		glProgramUniformMatrix3fv(m_ShaderHandle, GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	void Shader::SetUniformMat4(const char* name, const glm::mat4& value) const
	{
		glProgramUniformMatrix4fv(m_ShaderHandle, GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	uint32_t Shader::GetUniformLocation(const char* name) const
	{
		auto& mapLocation = m_UniformCache.find(name);
		LOG_ASSERT(mapLocation != m_UniformCache.end(), "Uniform could not found!");
		return mapLocation->second;
	}

	void Shader::CalculateUniformLocations()
	{
		GLint uniform_count = 0;
		glGetProgramiv(m_ShaderHandle, GL_ACTIVE_UNIFORMS, &uniform_count);

		if (uniform_count != 0)
		{
			GLint 	max_name_len = 0;
			GLsizei length = 0;
			GLsizei count = 0;
			GLenum 	type = GL_NONE;
			glGetProgramiv(m_ShaderHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

			std::unique_ptr<char[]> uniform_name = std::make_unique<char[]>(max_name_len);

			for (GLint i = 0; i < uniform_count; ++i)
			{
				glGetActiveUniform(m_ShaderHandle, i, max_name_len, &length, &count, &type, uniform_name.get());

				uint32_t location = glGetUniformLocation(m_ShaderHandle, uniform_name.get());

				m_UniformCache.emplace(std::make_pair(std::string(uniform_name.get(), length), location));
			}
		}
	}

	void Shader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const char* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Vertex shader compilation failed
			Core::Log::Error("Vertex Shader complation failed: {0}", infoLog.data());
			LOG_ASSERT(false);
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Fragment shader compilation failed
			Core::Log::Error("Fragment Shader compilation failed: {0}", infoLog.data());
			LOG_ASSERT(false);
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_ShaderHandle = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ShaderHandle, vertexShader);
		glAttachShader(m_ShaderHandle, fragmentShader);

		// Link our program
		glLinkProgram(m_ShaderHandle);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ShaderHandle, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ShaderHandle, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_ShaderHandle);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Shader linking failed
			Core::Log::Error("Shader Linking failed: {0}", infoLog.data());
			LOG_ASSERT(false);
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_ShaderHandle, vertexShader);
		glDetachShader(m_ShaderHandle, fragmentShader);
	}

	// ShaderLibrary for Shader caching

	ShaderLibrary* ShaderLibrary::s_Instance = nullptr;

	void ShaderLibrary::Init()
	{
		if (!s_Instance)
			s_Instance = new ShaderLibrary;
	}

	const std::shared_ptr<Shader>& ShaderLibrary::LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		const auto& shaderCacheIt = s_Instance->m_ShaderCache.find(name);

		if (shaderCacheIt != s_Instance->m_ShaderCache.end())
			return shaderCacheIt->second;

		const auto& insertedShaderIt = s_Instance->m_ShaderCache.insert(
			shaderCacheIt,
			std::make_pair(name, std::make_shared<Shader>(vertexSrc, fragmentSrc)));

		return insertedShaderIt->second;
	}

	const std::shared_ptr<Shader>& ShaderLibrary::LoadShader(const std::string& sourcePath)
	{
		std::string shaderSource = ReadFile(sourcePath);
		auto parsedSources = ParseShaderSource(shaderSource);
		return LoadShader(sourcePath, parsedSources[GL_VERTEX_SHADER], parsedSources[GL_FRAGMENT_SHADER]);
	}

	std::string ShaderLibrary::ReadFile(const std::string& sourcePath)
	{
		std::string source;
		std::ifstream shaderFile(sourcePath, std::ios::in | std::ios::binary);
		if (shaderFile)
		{
			shaderFile.seekg(0, std::ios::end);
			size_t size = shaderFile.tellg();
			if (size != -1)
			{
				source.resize(size);
				shaderFile.seekg(0, std::ios::beg);
				shaderFile.read(&source[0], size);
				shaderFile.close();
			}
			else
			{
				LOG_ASSERT(false, "Could not read from file!");
			}
		}
		else
		{
			// Could not open file
			LOG_ASSERT(false, "Could not open file!");
		}
		return source;
	}

	std::unordered_map<uint32_t, std::string> ShaderLibrary::ParseShaderSource(const std::string& source)
	{
		auto ShaderType = [](const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			else if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			return 0;
		};

		std::unordered_map<uint32_t, std::string> shaderSources;

		size_t begin = source.find("#type", 0);
		while (begin != std::string::npos)
		{
			size_t end = source.find_first_of("\r\n", begin);
			// Syntax error
			LOG_ASSERT(end != std::string::npos, "Shader program: Syntax Error!");
			begin = begin + strlen("#type") + 1;
			std::string type = source.substr(begin, end - begin);
			// Invalid shader type
			LOG_ASSERT(ShaderType(type), "Invalid Shader type!");

			begin = source.find_first_not_of("\r\n", end);
			//  Syntax error
			LOG_ASSERT(begin != std::string::npos, "Shader program: Syntax Error!");
			end = source.find("#type", begin);

			shaderSources[ShaderType(type)] = (end == std::string::npos) ? source.substr(begin) : source.substr(begin, end - begin);
			begin = end;
		}
		return shaderSources;
	}
}