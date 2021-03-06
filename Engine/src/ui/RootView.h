#pragma once

#include "control/ControlContainer.h"
#include "application/Window.h"

namespace Ghurund::UI {
    class RootView: public ControlContainer {
    private:
        Canvas* canvas;
        uint32_t backgroundColor = 0;
        Control* prevFocusedChild = nullptr;
        Control* capturedChild = nullptr;
        UIContext* context = nullptr;

        EventHandler<Window> focusChangedHandler = [this](Ghurund::Window& window) {
            if (!window.Visible || Focused) {
                if (prevFocusedChild) {
                    prevFocusedChild->requestFocus();
                    prevFocusedChild->release();
                    prevFocusedChild = nullptr;
                }
            } else {
                Control* focus = Focus;
                if (focus) {
                    while (focus->Focus)
                        focus = focus->Focus;
                    setPointer(prevFocusedChild, focus);
                    clearFocus();
                }
            }
#ifdef _DEBUG
            validate();
#endif
            return true;
        };

    public:
        RootView(UIContext& context, Canvas& canvas);

        ~RootView();

        inline uint32_t getBackgroundColor() const {
            return backgroundColor;
        }

        inline void setBackgroundColor(uint32_t color) {
            backgroundColor = color;
        }

        __declspec(property(get = getBackgroundColor, put = setBackgroundColor)) uint32_t BackgroundColor;

        virtual bool isFocused() const override {
            return context->Window.isFocused();
        }

        virtual void setCapturedChild(Control* control) override {
            this->capturedChild = control;
        }

        virtual UIContext* getContext() override {
            return context;
        }

        virtual void repaint() {
            context->Window.refresh();
        }

        virtual void invalidate();

        using Control::draw;

        void draw();

        virtual bool dispatchKeyEvent(const Ghurund::Input::KeyEventArgs& event) override;

        virtual bool dispatchMouseButtonEvent(const Ghurund::Input::MouseButtonEventArgs& event) override;

        virtual bool dispatchMouseMotionEvent(const Ghurund::Input::MouseMotionEventArgs& event) override;
    };
}