#pragma once

#include "LayoutManager.h"

namespace Ghurund::UI {
    class TextLayoutManager:public LayoutManager {
    private:
        void layoutFlowingControlsRight(ControlGroup& group, float width);

        void layoutFlowingControlsLeft(ControlGroup& group, float width);

        float measureWidth(ControlGroup& group);

        int measureHeight(ControlGroup& group, int width);

    public:
        bool reverseLayout = false;
        float verticalSpacing = 0.0f, horizontalSpacing = 0.0f;

        virtual const FloatSize measure(ControlGroup& group, ChildrenProvider& provider, float parentWidth, float parentHeight) override;

        virtual void layout(ControlGroup& group, ChildrenProvider& provider, float x, float y, float width, float height) override;
    };
}