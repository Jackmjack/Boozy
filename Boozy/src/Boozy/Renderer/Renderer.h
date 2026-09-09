#pragma once
#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/RenderCommand.h"

#include "Boozy/Renderer/OrthographicCamera.h"
#include "Boozy/Renderer/Shader.h"

#include <cstdint>

namespace Boozy {

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        inline static SceneData m_SceneData;
    };
}
