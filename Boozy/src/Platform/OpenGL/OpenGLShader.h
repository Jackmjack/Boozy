#pragma once

#include "Boozy/Renderer/Shader.h"

namespace Boozy {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;
    private:
        uint32_t m_RendererID;
    };

}


