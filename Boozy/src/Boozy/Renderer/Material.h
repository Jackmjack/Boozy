#pragma once

#include "Boozy/Renderer/Shader.h"
#include "Boozy/Renderer/Texture.h"

namespace Boozy {

    class Material
    {
    public:
        Material(const Ref<Shader>& shader, const glm::vec4& color, const Ref<Texture2D>& texture = nullptr);

        void SetShader(const Ref<Shader>& shader);
        void SetColor(const glm::vec4& color);
        void SetTexture(const Ref<Texture2D>& texture);
        void SetDoubleSided(bool doubleSided);

        const Ref<Shader>& GetShader() const { return m_Shader; }
        const glm::vec4& GetColor() const { return m_Color; }
        const Ref<Texture2D>& GetTexture() const { return m_Texture; }
        bool GetDoubleSided() const { return m_DoubleSided; }

        void Bind() const;

    private:
        Ref<Shader> m_Shader;
        glm::vec4 m_Color = glm::vec4(1.0f);
        Ref<Texture2D> m_Texture;
        bool m_DoubleSided = false;
    };

}

