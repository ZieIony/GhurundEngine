#include "Light.h"
#include "RenderingBatch.h"

namespace Ghurund {
    void Light::flatten(RenderingBatch & batch, XMFLOAT4X4 &transformation) {
        batch.addLight(*this);
    }
}
