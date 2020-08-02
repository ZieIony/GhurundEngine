// NativeDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "core/Object.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "D3DCompiler.lib")

#ifdef _WIN64
#pragma comment(lib, "GhurundEngine_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#else
#pragma comment(lib, "GhurundEngine_32.lib")
#pragma comment(lib, "PhysX_32.lib")
#pragma comment(lib, "PhysXCommon_32.lib")
#pragma comment(lib, "PhysXFoundation_32.lib")
#pragma comment(lib, "PhysXExtensions_static_32.lib")
#pragma comment(lib, "PhysXPvdSDK_static_32.lib")
#endif

using namespace Ghurund;

extern "C" __declspec(dllexport) void* getArrayItem(void** array, int index) {
    return array[index];
}

extern "C" __declspec(dllexport) void deleteObject(void* obj) {
    delete obj;
}

extern "C" __declspec(dllexport) void deleteArray(void* arr) {
    delete[] arr;
}

extern "C" __declspec(dllexport) void Object_delete(Object* obj) {
    delete obj;
}

extern "C" __declspec(dllexport) const Type* Object_getType(Object* _this) {
    return &_this->getType();
}
