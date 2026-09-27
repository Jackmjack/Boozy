#pragma once
#include "Boozy/Core/Core.h"
#include "Boozy/Renderer/RendererAPI.h"

namespace Boozy {

    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_RendererAPI->Init();
        }

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static const int32_t* GetViewport()
        {
            return s_RendererAPI->GetViewport();
        }

        inline static uint32_t GetFrameBuffer()
        {
            return s_RendererAPI->GetFrameBuffer();
        }

        inline static void BindFrameBuffer(uint32_t rendererID)
        {
            s_RendererAPI->BindFrameBuffer(rendererID);
        }

        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void ClearDepth()
        {
            s_RendererAPI->ClearDepth();
        }

        inline static void EnableDepthTest()
        {
            s_RendererAPI->EnableDepthTest();
        }

        inline static void DisableDepthTest()
        {
            s_RendererAPI->DisableDepthTest();
        }

        inline static void EnableFaceCulling()
        {
            s_RendererAPI->EnableFaceCulling();
        }

        inline static void DisableFaceCulling()
        {
            s_RendererAPI->DisableFaceCulling();
        }

        inline static void EnableBlending()
        {
            s_RendererAPI->EnableBlending();
        }

        inline static void DisableBlending()
        {
            s_RendererAPI->DisableBlending();
        }

        inline static void EnablePolygonOffset()
        {
            s_RendererAPI->EnablePolygonOffset();
        }

        inline static void DisablePolygonOffset()
        {
            s_RendererAPI->DisablePolygonOffset();
        }

        inline static void BindTexture(uint32_t slot, uint32_t rendererID)
        {
            s_RendererAPI->BindTexture(slot, rendererID);
        }

        inline static void BindTextureCube(uint32_t slot, uint32_t rendererID)
        {
            s_RendererAPI->BindTextureCube(slot, rendererID);
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

        inline static void DrawArrays(uint32_t vertexCount)
        {
            s_RendererAPI->DrawArrays(vertexCount);

        }
    private:
        static RendererAPI* s_RendererAPI;
    };
}
