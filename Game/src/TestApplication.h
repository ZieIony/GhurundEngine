﻿#include "core/math/MathUtils.h"
#include "ui/layout/LinearLayout.h"
#include "ui/layout/LinearLayout.h"
#include "ui/control/Space.h"
#include "ui/RootView.h"
#include "application/Application.h"
#include "application/SystemWindow.h"
#include "core/SharedPointer.h"
#include "ui/widget/menu/MenuItem.h"
#include "ui/widget/menu/Toolbar.h"
#include "ui/widget/tab/TabContainer.h"
#include "ui/widget/SplitLayout.h"
#include "TestRecycler.h"
#include "tool/LogPanel.h"
#include "TestControls.h"
#include "ui/widget/menu/PopupMenu.h"
#include "TestImageViews.h"
#include "TestFlowLayouts.h"
#include "LayoutEditorTab.h"
#include "DragTestTab.h"
#include "WindowsTestTab.h"
#include "audio/Sound.h"
#include "LoginTest.h"
#include "ui/LayoutLoader.h"
#include "ui/style/LightTheme.h"

#include "control/FpsText.h"

import Module;

namespace Game {
    using namespace Ghurund;
    using namespace Ghurund::Editor;

    class TestApplication:public Application {
    private:
        Theme* theme;
        Theme* menuTheme;
        UIContext* context;
        ModuleTest test;
        LayoutLoader layoutLoader;

        SharedPointer<FpsText> fps;

    public:
        void onInit() {
            /*testLevel = ghnew TestLevel(*this);
            LevelManager.setLevel(testLevel);

            Renderer.ClearColor = &makeColor(0xff7f7f7f);*/

            SystemWindow* window = nullptr;
            if (Settings.windowed) {
                window = ghnew OverlappedWindow(Timer);
            } else {
                window = ghnew FullscreenWindow(Timer);
            }

            window->initParameters(ParameterManager);

            window->Size = { Settings.width, Settings.height };
            SwapChain* swapChain = ghnew SwapChain();
            swapChain->init(Graphics, &Graphics2D, *window);
            window->SwapChain = swapChain;

            theme = ghnew LightTheme(ResourceContext, 0xff0078D7);
            menuTheme = ghnew LightTheme(ResourceContext, 0xff0078D7);
            layoutLoader.init(*theme, ResourceContext);
            context = ghnew UIContext(Graphics2D, *window, layoutLoader);

            Ghurund::UI::Canvas* canvas = ghnew Ghurund::UI::Canvas();
            canvas->init(Graphics2D);
            SharedPointer<Ghurund::UI::RootView> rootView = ghnew Ghurund::UI::RootView(*context, *canvas);
            rootView->Theme = theme;
            rootView->BackgroundColor = 0xffffffff;
            window->RootView = rootView;
            Windows.add(window);

            SharedPointer<ToolWindow> logWindow = ghnew ToolWindow();
            SharedPointer<LogPanel> logPanel = ghnew LogPanel();
            logWindow->Content = logPanel;
            logWindow->Title = L"Logs";
            /*Logger::init(ghnew CallbackLogOutput([this](LogType type, const tchar* log) {
                logPanel->addLog(type, copyStr(log));
                logPanel->repaint();
            }));*/

            SharedPointer<ColorView> redSurface = ghnew ColorView(0xffff0000);
            SplitLayoutPtr splitLayout = ghnew SplitLayout(Orientation::VERTICAL);
            splitLayout->Child1 = redSurface;
            splitLayout->Child2 = logWindow;

            SharedPointer<TestRecycler> testRecycler = ghnew TestRecycler(ResourceManager, ResourceContext, layoutLoader, *theme);
            testRecycler->Name = "test recycler";

            SharedPointer<TestImageViews> testImageViews = ghnew TestImageViews(ResourceContext, *theme);
            SharedPointer<TestFlowLayouts> testFlowLayouts = ghnew TestFlowLayouts(ResourceContext, layoutLoader);

            SharedPointer<TabContainer> tabLayout = ghnew TabContainer();
            tabLayout->Name = "tabs";

            SharedPointer<TestControls> column = ghnew TestControls(*theme, ResourceManager, ResourceContext, layoutLoader);
            column->Name = "controls tab";

            SharedPointer<LayoutEditorTab> layoutEditor = ghnew LayoutEditorTab();
            //layoutEditor->loadLayout(L"Game/layout.xml");
            SharedPointer<TestLoginScreen> loginTest = ghnew TestLoginScreen(*theme, ResourceContext, layoutLoader);
            SharedPointer<DragTestTab> dragTestTab = ghnew DragTestTab(layoutLoader);
            SharedPointer<WindowsTestTab> windowsTestTab = ghnew WindowsTestTab(*theme);

            tabLayout->Tabs = {
                ghnew TextTabItem(L"Login", loginTest),
                ghnew TextTabItem(L"RecyclerView", testRecycler),
                ghnew TextTabItem(L"ImageViews", testImageViews),
                ghnew TextTabItem(L"testFlowLayouts", testFlowLayouts),
                ghnew TextTabItem(L"controls", column),
                ghnew TextTabItem(L"SplitLayout", splitLayout),
                ghnew TextTabItem(L"layout editor", layoutEditor),
                ghnew TextTabItem(L"drag test", dragTestTab),
                ghnew TextTabItem(L"windows test", windowsTestTab)
            };
            tabLayout->Adapter = ghnew TextTabItemAdapter(*tabLayout, *theme);
            tabLayout->SelectedPosition = 0;

            SharedPointer<VerticalLayout> mainColumn = ghnew VerticalLayout();
            {
                BitmapImage* copyIcon = BitmapImage::makeFromImage(ResourceContext, L"icons/copy 18.png");
                BitmapImage* cutIcon = BitmapImage::makeFromImage(ResourceContext, L"icons/cut 18.png");
                BitmapImage* pasteIcon = BitmapImage::makeFromImage(ResourceContext, L"icons/paste 18.png");

                /*MenuBarPtr menuBar = ghnew MenuBar(layoutLoader);
                menuBar->Name = "menu bar";
                menuBar->Items = {
                    ghnew ButtonMenuItem(L"File", [](Control&) {
                        Logger::log(LogType::INFO, "File clicked\n");
                    }),
                    ghnew ButtonMenuItem(L"Edit", [this](Control& sender) {
                        Logger::log(LogType::INFO, "Edit clicked\n");
                        BitmapImage* copyIcon = BitmapImage::makeFromImage(ResourceContext, L"icons/copy 18.png");
                        BitmapImage* cutIcon = BitmapImage::makeFromImage(ResourceContext, L"icons/cut 18.png");
                        BitmapImage* pasteIcon = BitmapImage::makeFromImage(ResourceContext, L"icons/paste 18.png");
                        PopupMenu* menu = ghnew PopupMenu(*menuTheme, *Windows[0]);
                        menu->Items = {
                            ghnew ButtonMenuItem(L"Undo", [](Control&) {
                                Logger::log(LogType::INFO, "undo clicked\n");
                            }),
                            ghnew ButtonMenuItem(L"Redo", [](Control&) {
                                Logger::log(LogType::INFO, "redo clicked\n");
                            }),
                            ghnew SeparatorMenuItem(),
                            ghnew ButtonMenuItem(copyIcon, L"Copy", [](Control&) {
                                Logger::log(LogType::INFO, "copy clicked\n");
                            }),
                            ghnew ButtonMenuItem(cutIcon, L"Cut", [](Control&) {
                                Logger::log(LogType::INFO, "cut clicked\n");
                            }),
                            ghnew ButtonMenuItem(pasteIcon, L"Paste", [](Control&) {
                                Logger::log(LogType::INFO, "paste clicked\n");
                            })
                        };
                        auto pos = sender.PositionOnScreen;
                        //menu->Window->Position = { (LONG)pos.x, (LONG)(pos.y + sender.Size.height) };
                        menu->Visible = true;
                    }),
                    ghnew ButtonMenuItem(L"Help", [](Control&) {
                        Logger::log(LogType::INFO, "Help clicked\n");
                    }),
                    ghnew ButtonMenuItem(L"Very long menu text", [](Control&) {
                        Logger::log(LogType::INFO, "Very long menu text clicked\n");
                    })
                };*/

                SharedPointer<Toolbar> toolbar = ghnew Toolbar();
                toolbar->Name = "toolbar";
                toolbar->Items = {
                    ghnew ButtonMenuItem(copyIcon, L"copy", [](Control&) {
                        Logger::log(LogType::INFO, "copy clicked\n");
                    }),
                    ghnew ButtonMenuItem(cutIcon, L"cut", [](Control&) {
                        Logger::log(LogType::INFO, "cut clicked\n");
                    }),
                    ghnew ButtonMenuItem(pasteIcon, L"paste", [](Control&) {
                        Logger::log(LogType::INFO, "paste clicked\n");
                    })
                };

                auto statusBar = makeShared<StackLayout>();
                statusBar->PreferredSize.height = PreferredSize::Height::WRAP;
                SharedPointer<ColorView> statusBarBackground = ghnew ColorView();
                statusBarBackground->Name = "status bar";
                statusBarBackground->PreferredSize.height = 24;
                auto statusBarItems = makeShared<HorizontalLayout>();
                statusBarItems->Alignment = { Alignment::Horizontal::RIGHT, Alignment::Vertical::CENTER };
                statusBarItems->PreferredSize.height = PreferredSize::Height::WRAP;
                //fps = makeShared<FpsText>(theme->TextFormats[Theme::TEXTFORMAT_TEXT_PRIMARY], theme->ColorForegroundPrimaryOnBackground, Timer);
                //statusBarItems->Children = { fps };
                statusBar->Children = { statusBarBackground, statusBarItems };

                mainColumn->Children = { /*menuBar,*/ toolbar, tabLayout, statusBar };
            }
            rootView->Child = mainColumn;

            window->Visible = true;
            window->activate();
        }

        void onUninit() {
            delete context;
            delete theme;
            delete menuTheme;
            /*LevelManager.setLevel(nullptr);
            delete testLevel;*/
        }
    };
}
