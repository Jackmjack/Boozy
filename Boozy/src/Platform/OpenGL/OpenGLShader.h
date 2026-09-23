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

        virtual void SetBool(const std::string& name, bool value) override;

        virtual void SetInt(const std::string& name, int value) override;
        virtual void SetInt2(const std::string& name, const glm::ivec2& value) override;
        virtual void SetInt3(const std::string& name, const glm::ivec3& value) override;
        virtual void SetInt4(const std::string& name, const glm::ivec4& value) override;

        virtual void SetUInt(const std::string& name, uint32_t value) override;
        virtual void SetUInt2(const std::string& name, const glm::uvec2& value) override;
        virtual void SetUInt3(const std::string& name, const glm::uvec3& value) override;
        virtual void SetUInt4(const std::string& name, const glm::uvec4& value) override;

        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

        virtual void SetMat2(const std::string& name, const glm::mat2& matrix) override;
        virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

        virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) override;
        virtual void SetFloatArray(const std::string& name, const float* values, uint32_t count) override;
        virtual void SetFloat3Array(const std::string& name, const glm::vec3* values, uint32_t count) override;
        virtual void SetMat4Array(const std::string& name, const glm::mat4* values, uint32_t count) override;

    private:
        void UploadUniformBool(const std::string& name, bool value);

        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformInt2(const std::string& name, const glm::ivec2& value);
        void UploadUniformInt3(const std::string& name, const glm::ivec3& value);
        void UploadUniformInt4(const std::string& name, const glm::ivec4& value);

        void UploadUniformUInt(const std::string& name, uint32_t value);
        void UploadUniformUInt2(const std::string& name, const glm::uvec2& value);
        void UploadUniformUInt3(const std::string& name, const glm::uvec3& value);
        void UploadUniformUInt4(const std::string& name, const glm::uvec4& value);

        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat2(const std::string& name, const glm::mat2& matrix);
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

        void UploadIntArray(const std::string& name, const int* values, uint32_t count);
        void UploadFloatArray(const std::string& name, const float* values, uint32_t count);
        void UploadFloat3Array(const std::string& name, const glm::vec3* values, uint32_t count);
        void UploadMat4Array(const std::string& name, const glm::mat4* values, uint32_t count);

        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

        uint32_t m_RendererID;
        std::string m_Name;
    };

}


