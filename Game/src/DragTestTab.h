#pragma once

#include "ui/style/Theme.h"
#include "ui/layout/ManualLayout.h"
#include "ui/widget/button/Button.h"
#include "ui/widget/DragHelper.h"

using namespace Ghurund;
using namespace Ghurund::UI;

namespace Ghurund::Editor {
    class DragTestTab:public ManualLayout {
    private:
        DragHelper* helper;
        DragHelper* helper2;

    public:
        DragTestTab(LayoutLoader& loader) {
            /*TextButtonPtr tb = ghnew TextButton(ghnew TextButtonFlatLayout(loader));
            tb->Text = L"CANCEL";
            helper = ghnew DragHelper(*tb);
            TextButtonPtr tb2 = ghnew TextButton(ghnew TextButtonFlatLayout(loader));
            tb2->Text = L"TEST";
            helper2 = ghnew DragHelper(*tb2);
            Children = { tb, tb2 };*/
        }
    };
}
