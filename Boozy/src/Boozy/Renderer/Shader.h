#pragma once
#include <string>
#include <cstdint>
#include <glm/glm.hpp>

namespace Boozy {

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniformBool(const std::string& name, bool value) = 0;

        virtual void UploadUniformInt(const std::string& name, int value) = 0;
        virtual void UploadUniformInt2(const std::string& name, const glm::ivec2& value) = 0;
        virtual void UploadUniformInt3(const std::string& name, const glm::ivec3& value) = 0;
        virtual void UploadUniformInt4(const std::string& name, const glm::ivec4& value) = 0;

        virtual void UploadUniformUInt(const std::string& name, uint32_t value) = 0;
        virtual void UploadUniformUInt2(const std::string& name, const glm::uvec2& value) = 0;
        virtual void UploadUniformUInt3(const std::string& name, const glm::uvec3& value) = 0;
        virtual void UploadUniformUInt4(const std::string& name, const glm::uvec4& value) = 0;

        virtual void UploadUniformFloat(const std::string& name, float value) = 0;
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

        virtual void UploadUniformMat2(const std::string& name, const glm::mat2& matrix) = 0;
        virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}
