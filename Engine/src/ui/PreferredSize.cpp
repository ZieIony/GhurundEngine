#include "ghpch.h"
#include "PreferredSize.h"

namespace Ghurund::UI {
    const PreferredSize::Width& PreferredSize::Width::WRAP = PreferredSize::Width(PreferredSize::SpecialValues::WRAP);
    const PreferredSize::Height& PreferredSize::Height::WRAP = PreferredSize::Height(PreferredSize::SpecialValues::WRAP);
    const PreferredSize::Width& PreferredSize::Width::FILL = PreferredSize::Width(PreferredSize::SpecialValues::FILL);
    const PreferredSize::Height& PreferredSize::Height::FILL = PreferredSize::Height(PreferredSize::SpecialValues::FILL);
}