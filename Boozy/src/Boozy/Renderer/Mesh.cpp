#include "bzpch.h"
#include "Mesh.h"
#include "Boozy/Core/Log.h"

BZ_INIT_LOGGER("Renderer");

namespace Boozy
{
    Mesh::Mesh(const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, BufferLayout layout)
    {
        BZ_ASSERT(layout.GetStride() == sizeof(Vertex), "BufferLayout must match the Vertex structure");
        m_VertexArray = VertexArray::Create();

        m_VertexBuffer.reset(VertexBuffer::Create(reinterpret_cast<const float*>(vertices), layout.GetStride() * vertexCount));

        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        m_IndexBuffer.reset(IndexBuffer::Create(indices, indexCount));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    void Mesh::Bind() const
    {
        m_VertexArray->Bind();
        m_VertexBuffer->Bind();
        m_IndexBuffer->Bind();
    }
}