#include "Ghurund.h"
#include "core/io/DirectoryPath.h"
#include "core/logging/Formatter.h"

#include "DemoApplication.h"

#include "Dxgi1_6.h"
#include <dxgidebug.h>

#ifdef _WIN64
#pragma comment(lib, "GhurundEngine.Core_64.lib")
#pragma comment(lib, "GhurundEngine_64.lib")
#else
#pragma comment(lib, "GhurundEngine.Core_32.lib")
#pragma comment(lib, "GhurundEngine_32.lib")
#endif

using namespace Ghurund;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    _____________________checkMemory();

    Logger::init();
    Logger::log(LogType::INFO, _T("working dir: {}\n"), Ghurund::DirectoryPath(L".").AbsolutePath);

    HANDLE singleInstanceMutex = CreateMutex(nullptr, true, _T("Preview::DemoApplication"));
    bool alreadyRunning = GetLastError() == ERROR_ALREADY_EXISTS;

    if (alreadyRunning) {
        MessageBox(nullptr, _T("Application is already running."), _T("Preview::DemoApplication"), MB_OK | MB_ICONEXCLAMATION);
    } else {
        Demo::DemoApplication application;
        Settings settings;
        settings.parse(cmdLine);
        application.run(&settings);
    }

    CloseHandle(singleInstanceMutex);

    Logger::uninit();

#ifdef _DEBUG
    _____________________checkMemory();
#endif
    return 0;
}