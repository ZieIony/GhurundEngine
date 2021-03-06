#pragma once

#include "graphics/Renderer.h"
#include "graphics/texture/Image.h"

namespace Ghurund {
    class ThumbnailRenderer:public Object {
    private:
		Camera* camera = nullptr;
        Renderer* renderer = nullptr;
        Frame* frame = nullptr;
        RenderTarget* renderTarget = nullptr;
        DepthBuffer* depthBuffer = nullptr;

        ResourceContext* resourceContext = nullptr;

    protected:
        static const Ghurund::Type& GET_TYPE();

    public:
        ~ThumbnailRenderer() {
            if (camera)
                camera->release();
            delete renderer;
            delete frame;
        }

        void init(ResourceContext& context, uint32_t width, uint32_t height);

        Status render(DrawableComponent& entity, Image*& image);
        Status render(Mesh& mesh, Image*& image);
        Status render(Material& material, Image*& image);

        inline static const Ghurund::Type& TYPE = GET_TYPE();

        virtual const Ghurund::Type& getType() const override {
            return TYPE;
        }
    };
}