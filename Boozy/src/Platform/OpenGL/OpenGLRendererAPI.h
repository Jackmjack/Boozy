#pragma once
#include "Boozy/Renderer/RendererAPI.h"

namespace Boozy {

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual const int32_t* GetViewport() const override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void ClearDepth() override;

        virtual uint32_t GetFrameBuffer() const override;
        virtual void BindFrameBuffer(uint32_t rendererID) override;

        virtual void EnableDepthTest() override;
        virtual void DisableDepthTest() override;
        virtual void EnableFaceCulling() override;
        virtual void DisableFaceCulling() override;
        virtual void EnableBlending() override;
        virtual void DisableBlending() override;
        virtual void EnablePolygonOffset() override;
        virtual void DisablePolygonOffset() override;

        virtual void BindTexture(uint32_t slot, uint32_t rendererID) override;
        virtual void BindTextureCube(uint32_t slot, uint32_t rendererID) override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        virtual void DrawArrays(uint32_t vertexCount) override;
    };

}