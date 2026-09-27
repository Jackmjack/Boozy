#include "bzpch.h"
#include "Material.h"
#include "Boozy/Renderer/RenderCommand.h"

namespace Boozy {
    Material::Material(const Ref<Shader>& shader, const glm::vec4& color, const Ref<Texture2D>& texture)
        : m_Shader(shader), m_Color(color), m_Texture(texture), m_DoubleSided(false)
    {
    }

    void Material::SetShader(const Ref<Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::SetColor(const glm::vec4& color)
    {
        m_Color = color;
    }

    void Material::SetTexture(const Ref<Texture2D>& texture)
    {
        m_Texture = texture;
    }

    void Material::SetDoubleSided(bool doubleSided)
    {
        m_DoubleSided = doubleSided;
    }

    void Material::Bind() const
    {
        m_Shader->Bind();
        m_Shader->SetFloat4("u_Color", m_Color);

        if (m_DoubleSided) RenderCommand::DisableFaceCulling();
        else RenderCommand::EnableFaceCulling();
    }

}