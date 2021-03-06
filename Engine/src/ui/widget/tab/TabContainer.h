#pragma once

#include "TabItemAdapter.h"
#include "core/SharedPointer.h"
#include "ui/adapter/AdapterChildrenProvider.h"
#include "ui/adapter/RecyclerView.h"
#include "ui/layout/HorizontalLayoutManager.h"
#include "ui/widget/Widget.h"
#include "TabContainerBinding.h"

namespace Ghurund::UI {
    class TabContainer:public Widget<TabContainerBinding> {
    private:
        size_t selectedPosition = 0;
        Tab* selectedTab = nullptr;
        List<TabItem*> tabs;
        AdapterChildrenProvider<TabItem*, Tab>* provider;

    protected:
        virtual void onLayoutChanged() override {
            if (!Layout)
                return;
            provider = ghnew AdapterChildrenProvider<TabItem*, Tab>(*Layout->Tabs);
            provider->Items = ghnew ListItemSource<TabItem*>(tabs);
            Layout->Tabs->childrenProvider = provider;
        }

    public:
        class LayoutVerticalBottom;

        TabContainer();

        ~TabContainer();

        inline List<TabItem*>& getTabs() {
            return tabs;
        }

        inline void setTabs(const List<TabItem*>& tabs) {
            this->tabs.clear();
            this->tabs.addAll(tabs);
        }

        __declspec(property(get = getTabs, put = setTabs)) List<TabItem*>& Tabs;

        inline void setAdapter(TabItemAdapter* adapter) {
            provider->Adapters.deleteItems();
            provider->Adapters.add(adapter);
        }

        __declspec(property(put = setAdapter)) TabItemAdapter* Adapter;

        inline void setSelectedPosition(size_t position) {
            if (selectedTab)
                selectedTab->Layout->SelectableView->Selected = false;
            this->selectedPosition = position;
            if (Layout->Tabs->Children.Size > position) {
                setPointer(selectedTab, (Tab*)Layout->Tabs->Children[position]);
                selectedTab->Layout->SelectableView->Selected = true;
            }
            TabItem* tab = tabs[position];
            Layout->Content = tab->content;
            tab->content->invalidate();
        }

        inline size_t getSelectedPosition() const {
            return selectedPosition;
        }

        __declspec(property(get = getSelectedPosition, put = setSelectedPosition)) size_t SelectedPosition;
    };
}