#pragma once

#include "Ghurund.h"
#include "process.h"
#include "CriticalSection.h"
#include <queue>
#include <functional>
#include "Type.h"

namespace Ghurund {
    class Thread {
    private:
        HANDLE handle;
        DWORD threadId;
        bool finishing = false;
        CriticalSection section;
        HANDLE event;

        static DWORD WINAPI threadMain(void *arg) {
            Thread *thread = (Thread*)arg;
            thread->run();
            return 0;
        }

    protected:
        virtual void wakeUp() {}

    public:

        Thread() {
            event = CreateEvent(nullptr, false, false, nullptr);
        }

        void start() {
            handle = (HANDLE)CreateThread(NULL, 0, threadMain, this, 0, &threadId);
        }

        virtual void run() = 0;

        void wait() {
            WaitForSingleObjectEx(event, INFINITE, true);
        }

        void notify() {
            SetEvent(event);
        }

        bool isFinishing() {
            section.enter();
            bool result = finishing;
            section.leave();
            return result;
        }

        void finish() {
            section.enter();
            finishing = true;
            wakeUp();
            section.leave();
            WaitForSingleObject(handle, INFINITE);
            CloseHandle(handle);
        }

        DWORD getId() {
            return threadId;
        }

        HANDLE getHandle() {
            return handle;
        }
    };

    class APCThread:public Thread {
    private:
        static void CALLBACK wake(__in  ULONG_PTR arg) {}

    protected:
        virtual void wakeUp() {
            post(&wake, 0);
        }

    public:

        void post(PAPCFUNC apc, ULONG_PTR data) {
            QueueUserAPC(apc, getHandle(), data);
        }

        void run() {
            while(!isFinishing())
                ::SleepEx(INFINITE, true);
        }
    };
}