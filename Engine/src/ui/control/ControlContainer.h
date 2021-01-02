#pragma once

#include "ControlParent.h"

namespace Ghurund::UI {
    class ControlContainer: public ControlParent {
    private:
        Control* child = nullptr;
        bool previousReceiver = false;

    protected:
        static const Ghurund::Type& GET_TYPE() {
            static const Ghurund::Type TYPE = TypeBuilder(NAMESPACE_NAME, GH_STRINGIFY(ControlContainer))
                .withSupertype(__super::GET_TYPE());

            return TYPE;
        }

    public:
        ~ControlContainer() {
            if (child) {
                child->Parent = nullptr;
                child->release();
            }
        }

        inline Control* getChild() {
            return child;
        }

        inline const Control* getChild() const {
            return child;
        }

        inline void setChild(Control* child) {
            if (Focused)
                clearFocus();
            if (this->child)
                this->child->Parent = nullptr;
            setPointer(this->child, child);
            if (this->child)
                this->child->Parent = this;
        }

        __declspec(property(get = getChild, put = setChild)) Control* Child;

        virtual bool focusNext() override;

        virtual bool focusPrevious() override;

        virtual bool focusUp() override;

        virtual bool focusDown() override;

        virtual bool focusLeft() override;

        virtual bool focusRight() override;

        virtual void dispatchContextChanged() override;

        virtual void onMeasure(float parentWidth, float parentHeight) override;

        virtual void onLayout(float x, float y, float width, float height) override {
            if (child)
                child->layout(0, 0, width, height);
        }

        virtual void onUpdate(const uint64_t time) override {
            if (child)
                child->onUpdate(time);
        }

        virtual void onDraw(Canvas& canvas) override {
            if (child)
                child->draw(canvas);
        }

        virtual bool dispatchKeyEvent(const Input::KeyEventArgs& event) override;

        virtual bool dispatchMouseButtonEvent(const Input::MouseButtonEventArgs& event) override;

        virtual bool dispatchMouseMotionEvent(const Input::MouseMotionEventArgs& event) override;

        virtual bool dispatchMouseWheelEvent(const Input::MouseWheelEventArgs& event) override;

        virtual Control* find(const String& name);

        virtual Status load(LayoutLoader& loader, ResourceContext& context, const tinyxml2::XMLElement& xml) override;

#ifdef _DEBUG
        virtual String logTree();
#endif

        inline static const Ghurund::Type& TYPE = GET_TYPE();

        virtual const Ghurund::Type& getType() const override {
            return TYPE;
        }
    };
}