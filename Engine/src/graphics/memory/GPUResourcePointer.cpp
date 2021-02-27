#include "GPUResourcePointer.h"

#include "core/reflection/TypeBuilder.h"

namespace Ghurund {
    const Ghurund::Type& GPUResourcePointer::GET_TYPE() {
        static const Ghurund::Type TYPE = TypeBuilder(NAMESPACE_NAME, GH_STRINGIFY(GPUResourcePointer))
            .withSupertype(__super::GET_TYPE());

        return TYPE;
    }
}
