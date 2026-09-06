#pragma once

#include "Boozy/Renderer/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace Boozy {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const std::string& GetName() const override { return m_Name; }

        virtual void UploadUniformBool(const std::string& name, bool value) override;

        virtual void UploadUniformInt(const std::string& name, int value) override;
        virtual void UploadUniformInt2(const std::string& name, const glm::ivec2& value) override;
        virtual void UploadUniformInt3(const std::string& name, const glm::ivec3& value) override;
        virtual void UploadUniformInt4(const std::string& name, const glm::ivec4& value) override;

        virtual void UploadUniformUInt(const std::string& name, uint32_t value) override;
        virtual void UploadUniformUInt2(const std::string& name, const glm::uvec2& value) override;
        virtual void UploadUniformUInt3(const std::string& name, const glm::uvec3& value) override;
        virtual void UploadUniformUInt4(const std::string& name, const glm::uvec4& value) override;

        virtual void UploadUniformFloat(const std::string& name, float value) override;
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) override;

        virtual void UploadUniformMat2(const std::string& name, const glm::mat2& matrix) override;
        virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) override;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

        uint32_t m_RendererID;
        std::string m_Name;
    };

}


