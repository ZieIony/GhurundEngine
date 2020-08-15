#pragma once

#include "ui/gdi/GdiFont.h"
#include "Paint.h"
#include "gdi/GdiPath.h"

namespace Ghurund::UI {
    class Canvas {
    public:
        virtual ~Canvas() = 0 {};

        virtual void beginPaint() = 0;
        virtual void endPaint() = 0;
        virtual void clear(unsigned int color) = 0;

        virtual void drawRect(float x, float y, float width, float height, const Paint& paint) = 0;
        virtual void fillRect(float x, float y, float width, float height, const Paint& paint) = 0;

        virtual void drawPath(const GdiPath& path, const Paint& paint) = 0;

        virtual void drawLine(float x1, float y1, float x2, float y2, const Paint& paint) = 0;

        virtual void drawText(const String& text, float x, float y, float width, float height, const Ghurund::UI::Font& font, const Paint& paint) = 0;
        virtual void drawText2(const String& text, float x, float y, float width, float height, const Ghurund::UI::Font& font, const Paint& paint) = 0;

        virtual void drawImage(Gdiplus::Image& image, float x, float y, float width, float height) = 0;
        virtual void drawImage(Gdiplus::Image& image, float x, float y, Gdiplus::RectF src) = 0;

        virtual void translate(float x, float y) = 0;

        virtual void save() = 0;
        virtual void restore() = 0;

        virtual void clipPath(const GdiPath& path) = 0;
        virtual void clipRect(float x, float y, float width, float height) = 0;
    };
}