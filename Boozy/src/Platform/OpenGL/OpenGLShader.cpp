#include "bzpch.h"
#include "OpenGLShader.h"
#include "Boozy/Log.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

BZ_INIT_LOGGER("OpenGL");

namespace Boozy {

    OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        m_RendererID = glCreateProgram();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        glCompileShader(vertexShader);
        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);

            BZ_ERROR("Failed to compile vertex shader!");
            BZ_ASSERT(false, infoLog.data());
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            BZ_ERROR("Failed to compile fragment shader!");
            BZ_ASSERT(false, infoLog.data());
            return;
        }

        GLuint program = m_RendererID;
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            BZ_ERROR("Failed to link program!");
            BZ_ASSERT(false, infoLog.data());
            return;
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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


}

