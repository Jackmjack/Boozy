#include "bzpch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Boozy/Core/Log.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

BZ_INIT_LOGGER("OpenGL");

namespace Boozy {

    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        BZ_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_RendererID(0)
    {
        std::string shaderSource = ReadFile(filepath);
        auto shaderSources = PreProcess(shaderSource);
        Compile(shaderSources);

        auto lastSlash = filepath.find_last_of("/\\");
        if (lastSlash != std::string::npos)
        {
            auto lastDot = filepath.find_last_of(".");
            if (lastDot != std::string::npos && lastDot > lastSlash)
            {
                m_Name = filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);
            }
        }

        if (m_Name.empty())
            m_Name = filepath;
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name), m_RendererID(0)
    {
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader()
    {
        if (m_RendererID)
            glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetBool(const std::string& name, bool value)
    {
        OpenGLShader::UploadUniformBool(name, value);
    }

    void OpenGLShader::SetInt(const std::string & name, int value)
    {
        OpenGLShader::UploadUniformInt(name, value);
    }

    void OpenGLShader::SetInt2(const std::string & name, const glm::ivec2 & value)
    {
        OpenGLShader::UploadUniformInt2(name, value);
    }

    void OpenGLShader::SetInt3(const std::string & name, const glm::ivec3 & value)
    {
        OpenGLShader::UploadUniformInt3(name, value);
    }

    void OpenGLShader::SetInt4(const std::string & name, const glm::ivec4 & value)
    {
        OpenGLShader::UploadUniformInt4(name, value);
    }

    void OpenGLShader::SetUInt(const std::string & name, uint32_t value)
    {
        OpenGLShader::UploadUniformUInt(name, value);
    }

    void OpenGLShader::SetUInt2(const std::string & name, const glm::uvec2 & value)
    {
        OpenGLShader::UploadUniformUInt2(name, value);
    }

    void OpenGLShader::SetUInt3(const std::string & name, const glm::uvec3 & value)
    {
        OpenGLShader::UploadUniformUInt3(name, value);
    }

    void OpenGLShader::SetUInt4(const std::string & name, const glm::uvec4 & value)
    {
        OpenGLShader::UploadUniformUInt4(name, value);
    }

    void OpenGLShader::SetFloat(const std::string & name, float value)
    {
        OpenGLShader::UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat2(const std::string & name, const glm::vec2 & value)
    {
        OpenGLShader::UploadUniformFloat2(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string & name, const glm::vec3 & value)
    {
        OpenGLShader::UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string & name, const glm::vec4 & value)
    {
        OpenGLShader::UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat2(const std::string & name, const glm::mat2 & matrix)
    {
        OpenGLShader::UploadUniformMat2(name, matrix);
    }

    void OpenGLShader::SetMat3(const std::string & name, const glm::mat3 & matrix)
    {
        OpenGLShader::UploadUniformMat3(name, matrix);
    }

    void OpenGLShader::SetMat4(const std::string & name, const glm::mat4 & matrix)
    {
        OpenGLShader::UploadUniformMat4(name, matrix);
    }

    void OpenGLShader::SetIntArray(const std::string& name, const int* values, uint32_t count)
    {
        if (count == 0) return;
        OpenGLShader::UploadIntArray(name, values, count);
    }

    void OpenGLShader::SetFloatArray(const std::string & name, const float* values, uint32_t count)
    {
        if (count == 0) return;
        OpenGLShader::UploadFloatArray(name, values, count);
    }

    void OpenGLShader::SetFloat3Array(const std::string & name, const glm::vec3 * values, uint32_t count)
    {
        if (count == 0) return;
        OpenGLShader::UploadFloat3Array(name, values, count);
    }

    void OpenGLShader::UploadUniformBool(const std::string& name, bool value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value ? 1 : 0);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2i(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3i(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4i(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformUInt(const std::string& name, uint32_t value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1ui(location, value);
    }

    void OpenGLShader::UploadUniformUInt2(const std::string& name, const glm::uvec2& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2ui(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformUInt3(const std::string& name, const glm::uvec3& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3ui(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformUInt4(const std::string& name, const glm::uvec4& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4ui(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& matrix)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        glUseProgram(m_RendererID);
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadIntArray(const std::string& name, const int* values, uint32_t count)
    {
        glUseProgram(m_RendererID);
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadFloatArray(const std::string & name, const float* values, uint32_t count)
    {
        glUseProgram(m_RendererID);
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1fv(location, count, values);
    }

    void OpenGLShader::UploadFloat3Array(const std::string & name, const glm::vec3* values, uint32_t count)
    {
        glUseProgram(m_RendererID);
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3fv(location, count, glm::value_ptr(values[0]));
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            BZ_ERROR("Could not open file '{}'", filepath);
            BZ_ASSERT(false, "Could not open file!");
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            BZ_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            BZ_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        if (shaderSources.size() > 2)
        {
            BZ_ERROR("Only 2 shaders are supported for now");
            return;
        }

        GLuint program = glCreateProgram();
        std::array<GLuint, 2> glShaderIDs{};
        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);
                for (int i = 0; i < glShaderIDIndex; i++)
                    glDeleteShader(glShaderIDs[i]);

                glDeleteProgram(program);

                BZ_ERROR(infoLog.data());
                BZ_ASSERT(false, "Failed to compile shader!");
                return;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (int i = 0; i < glShaderIDIndex; i++)
            {
                glDeleteShader(glShaderIDs[i]);
            }

            BZ_ERROR(infoLog.data());
            BZ_ASSERT(false, "Failed to link program!");
            return;
        }

        for (int i = 0; i < glShaderIDIndex; i++)
        {
            glDetachShader(program, glShaderIDs[i]);
            glDeleteShader(glShaderIDs[i]);
        }

        m_RendererID = program;
    }

}

