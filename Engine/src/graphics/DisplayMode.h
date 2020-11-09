#pragma once

#pragma warning(push, 0)
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#pragma warning(pop)

#include "core/collection/List.h"
#include "core/string/String.h"
#include "core/Object.h"
#include "application/log/Logger.h"

#include <wrl.h>

namespace Ghurund {
    using namespace DirectX;
    using namespace Microsoft::WRL;

    class DisplayMode: public Object {
    private:
        unsigned int width, height;
        float refreshRate;
        DXGI_FORMAT format;

        static const Ghurund::Type& GET_TYPE() {
            static const Ghurund::Type TYPE = TypeBuilder(NAMESPACE_NAME, GH_STRINGIFY(DisplayMode))
                .withSupertype(__super::TYPE);

            return TYPE;
        }

    public:
        DisplayMode(DXGI_MODE_DESC mode) {
            width = mode.Width;
            height = mode.Height;
            refreshRate = (float)mode.RefreshRate.Numerator / mode.RefreshRate.Denominator;
            format = mode.Format;
        }

        unsigned int getWidth() {
            return width;
        }

        unsigned int getHeight() {
            return height;
        }

        float getRefreshRate() {
            return refreshRate;
        }

        DXGI_FORMAT getFormat() {
            return format;
        }

        inline static const Ghurund::Type& TYPE = GET_TYPE();

        virtual const Ghurund::Type& getType() const override {
            return TYPE;
        }
    };
}