#pragma once
#include "Boozy/Core/Core.h"
#include <string>
#include <unordered_map>
#include <cstdint>
#include <glm/glm.hpp>

namespace Boozy {

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;

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

        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}
