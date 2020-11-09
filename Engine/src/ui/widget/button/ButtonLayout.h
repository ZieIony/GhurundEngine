#pragma once

#include "ui/layout/StackLayout.h"
#include "ui/mixin/TextMixin.h"
#include "ui/mixin/BackgroundMixin.h"
#include "ui/mixin/BorderMixin.h"
#include "ui/mixin/PaddingMixin.h"
#include "ui/widget/ClickResponseView.h"
#include "ui/widget/Layout.h"

namespace Ghurund::UI {
    class ButtonLayout:public Layout {
    private:
        ClickableView* clickableView = nullptr;

    public:
        ~ButtonLayout() {
            if (clickableView)
                clickableView->release();
        }

        virtual void init() {
            clickableView = ghnew ClickResponseView();
        }

        inline ClickableView* getClickableView() {
            return clickableView;
        }

        __declspec(property(get = getClickableView)) ClickableView* ClickableView;
    };

}