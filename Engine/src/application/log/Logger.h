#pragma once

#include "Status.h"
#include "core/Enum.h"
#include "core/Noncopyable.h"
#include "core/threading/CriticalSection.h"
#include "LogType.h"
#include "LogOutput.h"
#include "Formatter.h"

#include "fmt/core.h"

#include <dbghelp.h>
#include <functional>

namespace Ghurund {
    class Logger:public Noncopyable {
    private:
        static HANDLE process;
        static SYMBOL_INFO* symbol;
        static IMAGEHLP_LINE line;
        static CriticalSection criticalSection;
        static LogType filterLevel;

        static LogOutput* logOutput;

        static address_t getAddress();

        static std::string getFileLine(address_t address);

        template<typename... Args>
        static void logVA(const LogType& type, const tchar* formatStr, Args&& ... args) {
            std::string fileLine = getFileLine(getAddress());

            std::string message = fmt::format(formatStr, args...);
            std::string log = message.ends_with(_T("\n")) ? fileLine + message : fileLine + message + _T("\n");

            writeLog(log.c_str(), log.size());
        }

        static void writeLog(const tchar* str, size_t length);

    public:
        static void init(LogOutput* output = nullptr);

        static void uninit();

        static void setFilter(const LogType& level) {
            filterLevel = level;
        }

        template<typename... Args>
        static void log(const LogType& type, const tchar* format, Args&& ... args) {
            if (((int)type.Value) < (int)filterLevel.Value)
                return;

            logVA(type, format, args...);
        }

        template<typename... Args>
        static Status log(const LogType& type, const Status status, const tchar* format, Args&& ... args) {
            if (((int)type.Value) < (int)filterLevel.Value)
                return status;

            logVA(type, format, args...);

            return status;
        }
    };

}