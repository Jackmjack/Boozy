#pragma once
#include "VertexArray.h"

#include <memory>
#include <glm/glm.hpp>

namespace Boozy {

    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        enum class API
        {
            None = 0, OpenGL = 1
        };

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_API; }
    private:
        static API s_API;
    };
}