#include "../../stdafx.h"
#include "application/Logger.h"

using namespace Ghurund;

extern "C" __declspec(dllexport) void Logger_init(void(__stdcall *onLogged)(const tchar *log)) {
    Logger::init(LogOutput::CUSTOM_CALLBACK, nullptr, onLogged);
}

extern "C" __declspec(dllexport) void Logger_log(LogTypeEnum type, const tchar *log) {
    Logger::log(LogType::VALUES[type], log);
}
