#pragma once

#include "Boozy/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <string>

namespace Boozy {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
    private:
        uint32_t m_RendererID;
    };

}


