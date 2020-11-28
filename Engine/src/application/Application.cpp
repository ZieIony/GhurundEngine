#include "Application.h"
#include "WindowType.h"
#include "core/threading/FunctionQueue.h"
#include "ui/RootView.h"
#include "ui/Canvas.h"

#include <time.h>

namespace Ghurund {
    void Application::init() {
        CoInitialize(nullptr);

        // engine
        graphics = ghnew Ghurund::Graphics();
        graphics->init();
        graphics2d = ghnew Ghurund::UI::Graphics2D();
        graphics2d->init(*graphics);
        audio = ghnew Audio::Audio();
        audio->init();
        physics = ghnew Ghurund::Physics();
        physics->init();
        parameterManager = ghnew Ghurund::ParameterManager();

        timer = ghnew Ghurund::Timer();
        scriptEngine = ghnew Ghurund::ScriptEngine();
        scriptEngine->init(*timer);
        resourceManager = ghnew Ghurund::ResourceManager();
        resourceManager->Libraries.add(ResourceManager::ENGINE_LIB_NAME, DirectoryPath(L"."));

        resourceContext = ghnew Ghurund::ResourceContext(*graphics, *graphics2d, *audio, *parameterManager, *scriptEngine, *physics, *resourceManager);
        resourceContext->init();
        asyncResourceContext = ghnew Ghurund::ResourceContext(*graphics, *graphics2d, *audio, *parameterManager, *scriptEngine, *physics, *resourceManager);
        asyncResourceContext->init();
        //parameterManager->initDefaultTextures(*resourceContext);

        // app
        functionQueue = ghnew Ghurund::FunctionQueue();
        renderer = ghnew Ghurund::Renderer();
        renderer->init(*resourceContext);

        client = ghnew Ghurund::Client(*functionQueue);
        client->init();
    }

    void Application::uninit() {
        windows.clear();

        delete scriptEngine;

        if (client->isConnected())
            client->disconnect();
        delete client;

        delete renderer;

        delete timer;
        delete parameterManager;
        delete asyncResourceContext;
        delete resourceContext;
        delete resourceManager;

        delete functionQueue;
        delete physics;
        delete audio;
        delete graphics2d;
        delete graphics;

        CoUninitialize();
    }

    void Application::run(const Ghurund::Settings* settings) {
        if (settings)
            this->settings = *settings;

        init();
        onInit();

        while (windows.Size != 0) {
            //while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            handleMessages();
            FunctionQueue.invoke();
            update();
            for (auto window : windows) {
                Frame& frame = window->SwapChain->CurrentFrame;
                CommandList& commandList = renderer->startFrame(frame);
                //levelManager.draw(commandList);
                window->OnUpdate(*timer);
                frame.flush();
                graphics2d->beginPaint(frame.RenderTarget);
                window->OnPaint();
                if (graphics2d->endPaint() != Status::OK)
                    break;
                if (renderer->finishFrame(frame) != Status::OK)
                    break;
                if (window->SwapChain->present() != Status::OK)
                    break;
            }
        }

        onUninit();
        uninit();
    }

    void Application::handleMessages() {
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                for (auto w : windows) {
                    if (w->Handle == msg.hwnd) {
                        windows.remove(w);
                        delete w;
                        break;
                    }
                }
            }
        }
    }

    void Application::update() {
        timer->tick();

        double dt = timer->FrameTime;	// TODO: constant dt

        resourceManager->reload();

        input.dispatchEvents(levelManager);

        scriptEngine->update(dt);

        onUpdate();

        input.clearEvents();

        //onDraw();
    }

    void Application::reset() {
        onUninit();
        onInit();
    }
}