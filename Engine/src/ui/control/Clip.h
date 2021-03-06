#pragma once

#include "ControlContainer.h"
#include "ui/Shape.h"

namespace Ghurund::UI {
    class Clip:public ControlContainer {
    private:
        Shape* shape = nullptr;

    protected:
        static const Ghurund::Type& GET_TYPE();

    public:
        ~Clip() {
            delete shape;
        }

        inline Shape* getShape() {
            return shape;
        }

        inline void setShape(Shape* shape) {
            this->shape = shape;
        }

        __declspec(property(get = getShape, put = setShape)) Shape* Shape;

        virtual void onLayout(float x, float y, float width, float height) override {
            __super::onLayout(x, y, width, height);
            if (shape)
                shape->Bounds = FloatRect{ 0, 0, width, height };
        }

        virtual void onDraw(Canvas& canvas) override;

        virtual Status load(LayoutLoader& loader, const tinyxml2::XMLElement& xml) override;

        inline static const Ghurund::Type& TYPE = GET_TYPE();

        virtual const Ghurund::Type& getType() const override {
            return TYPE;
        }
    };
}