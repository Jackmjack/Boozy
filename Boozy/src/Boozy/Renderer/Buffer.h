#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <initializer_list>

namespace Boozy {

    enum class ShaderDataType
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    uint32_t ShaderDataTypeSize(ShaderDataType type);

    class BufferElement
    {
    public:
        std::string Name;
        ShaderDataType Type = ShaderDataType::None;
        uint32_t Offset = 0;
        uint32_t Size = 0;
        bool Normalized = false;

        BufferElement() {}

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const;
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(const std::initializer_list<BufferElement>& element)
            : m_Elements(element)
        {
            CalculateOffsetAndStride();
        }

        inline uint32_t GetStride() const { return m_Stride; }
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    private:
        void CalculateOffsetAndStride()
        {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static VertexBuffer* Create(const float* vertices, uint32_t size);
    };

    class IndexBuffer
    {
    public:

        virtual ~IndexBuffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(const uint32_t* indices, uint32_t count);
    };
}