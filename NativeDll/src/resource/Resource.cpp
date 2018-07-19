#include "resource/Resource.h"

using namespace Ghurund;

extern "C" __declspec(dllexport) Status Resource_load(Resource *_this, ResourceManager *manager, const tchar *fileName) {
    return _this->load(*manager, fileName);
}

extern "C" __declspec(dllexport) Status Resource_save(Resource *_this, ResourceManager *manager, const tchar *fileName) {
    return _this->save(*manager, fileName);
}

extern "C" __declspec(dllexport) const Array<ResourceFormat> *Resource_getFormats(Resource *_this) {
    return &_this->getFormats();
}

extern "C" __declspec(dllexport) const ResourceFormat *Resource_getDefaultFormat(Resource *_this) {
    return &_this->getDefaultFormat();
}

extern "C" __declspec(dllexport) const tchar *ResourceFormat_getExtension(ResourceFormat *_this) {
    return copyStr(_this->getExtension());
}

extern "C" __declspec(dllexport) const Array<const ResourceFormat*> *ResourceFormat_getValues() {
    return &ResourceFormat::getValues();
}

extern "C" __declspec(dllexport) bool Resource_canSave(ResourceFormat *_this) {
    return _this->canSave();
}

extern "C" __declspec(dllexport) bool Resource_canLoad(ResourceFormat *_this) {
    return _this->canLoad();
}
