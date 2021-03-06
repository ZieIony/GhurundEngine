#include "ghcpch.h"
#include "core/logging/Logger.h"

#include "Common.h"

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <dbghelp.h>

namespace Ghurund {
    using namespace std;

    HANDLE Logger::process = {};
    _SYMBOL_INFO* Logger::symbol = nullptr;
    CriticalSection Logger::criticalSection;
    LogTypeEnum Logger::filterLevel = LogType::INFO.Value;
    LogOutput* Logger::logOutput = nullptr;

    address_t Logger::getAddress() {
        constexpr int frames = 1;
        void* stack;
        WORD numberOfFrames = CaptureStackBackTrace(3, frames, &stack, NULL);

        return (address_t)stack;
    }

    std::basic_string<tchar> Logger::getFileLine(address_t address) {
        IMAGEHLP_LINE line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE);
        DWORD64 displacement = 0;
        if (!SymFromAddr(process, address, &displacement, symbol))
            return fmt::format(_T("[{}]"), address);

        DWORD displacement2 = 0;
        if (!SymGetLineFromAddr(process, address, &displacement2, &line))
            return fmt::format(_T("[{}:{}]"), address, symbol->Name);

        return fmt::format(_T("{0}({1:d}): [{2:x} {3}(..)]"), line.FileName, line.LineNumber, address, symbol->Name);
    }

    void Logger::init(std::unique_ptr<LogOutput> output) {
        if (logOutput)
            uninit();

#ifdef _DEBUG
        filterLevel = LogType::INFO.Value;
#else
        filterLevel = LogType::ERR0R.Value;
#endif

        if (symbol == nullptr) {
            constexpr int frames = 1;
            process = GetCurrentProcess();
            SymInitialize(process, NULL, TRUE);

            constexpr int symbolStructSize = sizeof(SYMBOL_INFO) + (MAX_SYM_NAME - 1) * sizeof(tchar);
            symbol = (SYMBOL_INFO*)(ghnew char[symbolStructSize]);
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol->MaxNameLen = MAX_SYM_NAME;
        }

        if (output) {
            logOutput = output.release();
        } else {
            logOutput = ghnew SystemConsoleLogOutput();
        }
    }

    void Logger::uninit() {
        if (symbol != nullptr) {
            delete[] symbol;
            symbol = nullptr;
            SymCleanup(process);
            process = {};
        }

        delete logOutput;
        logOutput = nullptr;
    }
}