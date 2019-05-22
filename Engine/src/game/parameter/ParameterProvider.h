#pragma once

#include "Parameter.h"
#include "collection/PointerArray.h"
#include "ParameterManager.h"

namespace Ghurund {
    class ResourceContext;
    class ResourceManager;
    class DirectoryPath;
    class MemoryInputStream;
    class MemoryOutputStream;
    
    enum class LoadOption;
    enum class SaveOption;

    class ParameterProvider {
    protected:
        Status loadParameters(ResourceManager& resourceManager, ResourceContext& context, const DirectoryPath& workingDir, MemoryInputStream& stream, LoadOption options);
        Status saveParameters(ResourceManager& resourceManager, ResourceContext& context, const DirectoryPath& workingDir, MemoryOutputStream& stream, SaveOption options) const;

    public:
        virtual ~ParameterProvider() = default;

        virtual void initParameters(ParameterManager& parameterManager) {}

        virtual void updateParameters() {}

        virtual const PointerArray<Parameter*>& getParameters() const = 0;

        __declspec(property(get = getParameters)) PointerArray<Parameter*>& Parameters;

        Parameter* getParameter(const ASCIIString& name) {
            for (Parameter* p : Parameters) {
                if (p != nullptr && p->Name == name)
                    return p;
            }

            return nullptr;
        }
    };
}