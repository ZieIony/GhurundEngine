#pragma once

#include "MusicPlayer.h"

#include "core/SharedPointer.h"
#include "ui/control/ScrollView.h"
#include "ui/control/TextBlock.h"
#include "ui/layout/LinearLayout.h"
#include "ui/widget/button/CheckBox.h"
#include "ui/widget/button/RadioButton.h"
#include "ui/widget/button/RadioGroup.h"
#include "ui/widget/button/Button.h"
#include "ui/widget/button/Button.h"
#include "ui/widget/text/TextField.h"
#include "ui/widget/ProgressBar.h"
#include "ui/widget/menu/DropDown.h"

using namespace Ghurund;
using namespace Ghurund::UI;
using namespace Ghurund::Editor;

class TestControls:public ScrollView {
private:
    RadioGroup radioGroup;
    SharedPointer<MusicPlayer> player;

public:
    TestControls(Ghurund::UI::Theme& theme, ResourceManager& resourceManager, ResourceContext& context, LayoutLoader& loader) {
        Name = "controls tab";

        SharedPointer<VerticalLayout> verticalLayout = ghnew VerticalLayout();
        verticalLayout->PreferredSize.height = PreferredSize::Height::WRAP;

        {
            SharedPointer<TextFormat> latoLight = ghnew Ghurund::UI::TextFormat(L"fonts/lato_light.ttf", L"Lato Light", 40, FW_LIGHT, false);
            latoLight->init(context.Graphics2D);
            SharedPointer<TextBlock> textView = ghnew TextBlock(L"big light text", latoLight);
            SharedPointer<TextView> textView2 = ghnew TextView();
            textView2->PreferredSize.width = PreferredSize::Width::FILL;
            textView2->TextFormat = theme.TextFormats[Theme::TEXTFORMAT_TEXT_SECONDARY];
            textView2->Text = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

            auto p = makeShared<PaddingContainer>();
            p->Padding.All = 20;
            PointerList<Control*> controls;
            if (loader.load(FilePath(L"layouts/ButtonAccent.xml"), controls) == Status::OK) {
                auto hwButton = makeShared<Button>();
                hwButton->Layout = std::unique_ptr<ButtonBinding>(ghnew ButtonBinding(controls[0]));
                auto hwButtonText = makeShared<TextBlock>();
                hwButtonText->Text = L"HELLO WORLD";
                hwButton->Content = hwButtonText;
                p->Child = hwButton;
            }

            SharedPointer<TextField> textField = ghnew TextField();
            textField->Text = L"type here";
            SharedPointer<TextField> textField2 = ghnew TextField();
            textField2->Text = L"type here too";

            /*SharedPointer<CheckBox> checkBox = ghnew CheckBox();
            checkBox->Text = L"check me";

            RadioTextButtonPtr radioButton = ghnew RadioTextButton();
            radioButton->Text = L"option 1";
            RadioTextButtonPtr radioButton2 = ghnew RadioTextButton();
            radioButton2->Text = L"option 2";
            radioGroup = { radioButton, radioButton2 };*/

            SharedPointer<HorizontalLayout> horizontalLayout = ghnew HorizontalLayout();
            horizontalLayout->PreferredSize.height = PreferredSize::Height::WRAP;
            for (size_t i : {0, 1, 2, 3}) {
                SharedPointer<Button> imageButton = ghnew Button();
                SharedPointer<BitmapImage> saveIcon = BitmapImage::makeFromImage(context, L"icons/icon save 32.png");
                //imageButton->Image = makeShared<BitmapImageDrawable>(saveIcon);
                horizontalLayout->Children.add(imageButton);
            }

            player = ghnew MusicPlayer(theme);

            /*TextButtonPtr flatButton = ghnew TextButton(ghnew TextButtonFlatLayout(loader));
            flatButton->Text = L"Submit";
            TextButtonPtr accentButton = ghnew TextButton(ghnew TextButtonAccentLayout(loader));
            accentButton->Text = L"OK";*/

            SharedPointer<Separator> separator = ghnew Separator();
            separator->Style = theme.Styles[Theme::STYLE_SEPARATOR_HORIZONTAL];

            SharedPointer<ProgressBar> progressBar = ghnew ProgressBar();
            progressBar->Style = theme.Styles[Theme::STYLE_PROGRESSBAR];
            progressBar->Indeterminate = true;

            verticalLayout->Children = {
                textView,
                textView2,
                p,
                textField,
                textField2,
                makeShared<Space>(),
                /*checkBox,
                makeShared<Space>(),
                radioButton,
                radioButton2,*/
                separator,
                horizontalLayout,
                makeShared<Space>(),
                player,
                makeShared<Space>(),
                /*flatButton,
                makeShared<Space>(),
                accentButton,
                makeShared<Space>(),*/
                progressBar
            };
        }

        Child = verticalLayout;
    }
};