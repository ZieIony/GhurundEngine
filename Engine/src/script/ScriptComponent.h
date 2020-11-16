#pragma once

#include "ecs/Component.h"
#include "Script.h"

namespace Ghurund {
    class ScriptComponent:public Component {
    private:
        Ghurund::Script* script = nullptr;

    public:
        ScriptComponent(Ghurund::Script* script) {
            Script = script;
        }

        Ghurund::Script* getScript() {
            return script;
        }

        void setScript(Ghurund::Script* script) {
            setPointer(this->script, script);
        }

        __declspec(property(get = getScript, put = setScript)) Ghurund::Script* Script;
    };
}