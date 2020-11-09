#include "ClickResponseView.h"

namespace Ghurund::UI {
    void ClickResponseView::update(const Timer& timer) {
        uint32_t dt = timer.TimeMs - startTime;
        float percent = Pressed ? 1 : (1 - std::min(dt, length) / (float)length);
        finishedAnimating = percent == 0;
        int32_t alpha = (int32_t)(percent * ((color >> 24) & 0xff));
        paint.Color = ((alpha & 0xff) << 24) | (color & 0xffffff);
    }
}
