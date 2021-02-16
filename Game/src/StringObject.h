#pragma once

#include "core/SharedPointer.h"
#include "core/string/String.h"
#include "ui/adapter/RecyclerView.h"
#include "ui/control/Clip.h"
#include "ui/control/ColorView.h"
#include "ui/control/PaddingContainer.h"
#include "ui/control/TextBlock.h"
#include "ui/drawable/BitmapImageDrawable.h"
#include "ui/layout/LinearLayout.h"
#include "ui/widget/ClickResponseView.h"
#include "ui/widget/button/Button.h"
#include "ui/style/Theme.h"

using namespace Ghurund;
using namespace Ghurund::UI;

enum class StringObjectType {
    ITEM, HEADER
};

struct StringObject {
    StringObjectType type;

    virtual ~StringObject() {}
};

struct StringObjectItem:public StringObject {
    Ghurund::WString text, subtext;
    BitmapImage* image;

    StringObjectItem(const Ghurund::WString& text, const Ghurund::WString& subtext, BitmapImage* image):text(text), subtext(subtext) {
        this->image = image;
        if (image)
            image->addReference();
        type = StringObjectType::ITEM;
    }

    ~StringObjectItem() {
        image->release();
    }
};

struct StringObjectHeader:public StringObject {
    Ghurund::WString text;

    StringObjectHeader(const Ghurund::WString& text):text(text) {
        type = StringObjectType::HEADER;
    }
};

class StringObjectHeaderRow:public StackLayout {
private:
    SharedPointer<TextBlock> tv;
    SharedPointer<PaddingContainer> padding;

public:
    StringObjectHeaderRow(Ghurund::UI::Theme& theme) {
        preferredSize.height = PreferredSize::Height::WRAP;

        padding = ghnew PaddingContainer();
        {
            padding->PreferredSize.width = PreferredSize::Width::FILL;
            padding->Padding = { 16.0f, 4.0f };

            tv = ghnew TextBlock();
            tv->Style = theme.Styles[Theme::STYLE_TEXTBLOCK_HEADER];
            tv->PreferredSize.width = PreferredSize::Width::FILL;
            padding->Child = tv;

        }
        ColorView* colorView = ghnew ColorView(theme.Colors[Theme::COLOR_BACKGR0UND]);
        Children = {
            colorView,
            padding
        };
        colorView->release();
    }

    const Ghurund::WString& getText() {
        return tv->Text;
    }

    void setText(const Ghurund::WString& text) {
        tv->Text = text;
    }

    __declspec(property(get = getText, put = setText)) const WString& Text;
};

class StringObjectItemRow:public StackLayout {
private:
    TextBlock* tv;
    TextBlock* tv2;
    ImageView* imageView;

public:
    StringObjectItemRow(ResourceContext& context, LayoutLoader& loader, Ghurund::UI::Theme& theme) {
        preferredSize.height = PreferredSize::Height::WRAP;
        SharedPointer<ColorView> colorView = ghnew ColorView(theme.Colors[Theme::COLOR_BACKGR0UND]);

        SharedPointer<PaddingContainer> padding = ghnew PaddingContainer();
        {
            padding->PreferredSize.width = PreferredSize::Width::FILL;
            padding->Padding = { 16.0f, 8.0f };

            SharedPointer<HorizontalLayout> row = ghnew HorizontalLayout();
            {
                row->PreferredSize.height = PreferredSize::Height::WRAP;

                SharedPointer<Clip> clip = ghnew Clip();
                {
                    clip->Shape = ghnew Ghurund::UI::RoundRect(context.Graphics2D, 4.0f);

                    imageView = ghnew ImageView();
                    imageView->PreferredSize = { 48, 48 };
                    clip->Child = imageView;
                }

                SharedPointer<VerticalLayout> column = ghnew VerticalLayout();
                {
                    column->PreferredSize.height = PreferredSize::Height::WRAP;
                    column->Alignment.horizontal = Alignment::Horizontal::RIGHT;

                    tv = ghnew TextBlock();
                    tv->Style = theme.Styles[Theme::STYLE_TEXTBLOCK_PRIMARY];
                    tv->PreferredSize.width = PreferredSize::Width::FILL;
                    tv2 = ghnew TextBlock();
                    tv->Style = theme.Styles[Theme::STYLE_TEXTBLOCK_SECONDARY];
                    tv2->PreferredSize.width = PreferredSize::Width::FILL;
                    auto tb = makeShared<Button>();
                    auto tbt = makeShared<TextBlock>();
                    tb->Content = tbt;
                    tbt->Text = L"CANCEL";
                    column->Children = { tv, tv2, tb };
                }

                row->Children = { clip, makeShared<Space>(16.0f), column };
            }
            padding->Child = row;
            SharedPointer<ClickResponseView> responseView = ghnew ClickResponseView();

            Children = { colorView, responseView, padding };
        }
    }

    ~StringObjectItemRow() {
        tv->release();
        tv2->release();
        imageView->release();
    }

    const Ghurund::WString& getText() const {
        return tv->Text;
    }

    void setText(const Ghurund::WString& text) {
        tv->Text = text;
    }

    __declspec(property(get = getText, put = setText)) const WString& Text;

    const WString& getSubtext() const {
        return tv2->Text;
    }

    void setSubtext(const WString& text) {
        tv2->Text = text;
    }

    __declspec(property(get = getSubtext, put = setSubtext)) const WString& Subtext;

    inline void setImage(ImageDrawable* image) {
        imageView->Image = image;
    }

    inline ImageDrawable* getImage() {
        return imageView->Image;
    }

    __declspec(property(get = getImage, put = setImage)) ImageDrawable* Image;

};

class StringHeaderAdapter:public ItemAdapter<StringObject*, Control> {
private:
    Theme& theme;

public:
    StringHeaderAdapter(Theme& theme):theme(theme) {}

    virtual bool canHandleItem(StringObject* const& item, size_t position) const override {
        return item->type == StringObjectType::HEADER;
    }

    virtual Control* makeControl() const override {
        return ghnew StringObjectHeaderRow(theme);
    }

    virtual void bind(Control& control, StringObject* const& item, size_t position) const override {
        StringObjectHeaderRow& sor = (StringObjectHeaderRow&)control;
        StringObjectHeader* strObj = (StringObjectHeader*)item;
        sor.Text = strObj->text;
    }
};

class StringItemAdapter:public ItemAdapter<StringObject*, Control> {
private:
    Theme& theme;
    ResourceContext& context;
    LayoutLoader& loader;

public:
    StringItemAdapter(ResourceContext& context, LayoutLoader& loader, Theme& theme):context(context), loader(loader), theme(theme) {}

    virtual bool canHandleItem(StringObject* const& item, size_t position) const override {
        return item->type == StringObjectType::ITEM;
    }

    virtual Control* makeControl() const override {
        return ghnew StringObjectItemRow(context, loader, theme);
    }

    virtual void bind(Control& control, StringObject* const& item, size_t position) const override {
        StringObjectItemRow& sor = (StringObjectItemRow&)control;
        StringObjectItem* strObj = (StringObjectItem*)item;
        sor.Text = strObj->text;
        sor.Subtext = strObj->subtext;
        sor.Image = makeShared<BitmapImageDrawable>(strObj->image);
    }
};
