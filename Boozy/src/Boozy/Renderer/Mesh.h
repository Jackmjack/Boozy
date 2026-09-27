#pragma once

#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/Vertex.h"
#include "Boozy/Renderer/VertexArray.h"
#include "Boozy/Renderer/Buffer.h"

#include <cstdint>
#include <glm/glm.hpp>

namespace Boozy
{

    class Mesh
    {
    public:
        Mesh(const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount, BufferLayout layout);

        void Bind() const;

        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
    private:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

}
