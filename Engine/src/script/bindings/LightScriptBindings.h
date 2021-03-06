#pragma once

#include "ScriptBindings.h"
#include "graphics/entity/Light.h"

namespace Ghurund {

    class LightScriptBindings:ScriptBindings<Light> {
    public:
        static void registerClass(asIScriptEngine& engine) {
            ScriptBindings<Light>::registerRefClass(engine, Light::TYPE.Name);
        }
    };

}