#pragma once

#include "ui/Theme.h"
#include "ui/layout/ManualLayout.h"
#include "ui/widget/button/TextButton.h"
#include "ui/widget/DragHelper.h"

using namespace Ghurund;
using namespace Ghurund::UI;

namespace Ghurund::Editor {
    class DragTestTab:public ManualLayout {
    private:
        DragHelper* helper;
        DragHelper* helper2;

    public:
        DragTestTab(Theme& theme) {
            TextButtonPtr tb = ghnew TextButton(ghnew TextButtonFlatLayout(theme));
            tb->Text = "CANCEL";
            helper = ghnew DragHelper(*tb);
            TextButtonPtr tb2 = ghnew TextButton(ghnew TextButtonFlatLayout(theme));
            tb2->Text = "TEST";
            helper2 = ghnew DragHelper(*tb2);
            Children = { tb, tb2 };
        }
    };
}
