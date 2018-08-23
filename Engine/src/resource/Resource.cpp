#include "Resource.h"

namespace Ghurund {
    LoadOption operator |(LoadOption lhs, LoadOption rhs) {
        return (LoadOption)((std::underlying_type<LoadOption>::type)lhs|(std::underlying_type<LoadOption>::type)rhs);
    }

    bool operator &(LoadOption lhs, LoadOption rhs) {
        return (std::underlying_type<LoadOption>::type)lhs&(std::underlying_type<LoadOption>::type)rhs;
    }

    SaveOption operator |(SaveOption lhs, SaveOption rhs) {
        return (SaveOption)((std::underlying_type<SaveOption>::type)lhs|(std::underlying_type<SaveOption>::type)rhs);
    }

    bool operator &(SaveOption lhs, SaveOption rhs) {
        return (std::underlying_type<SaveOption>::type)lhs&(std::underlying_type<SaveOption>::type)rhs;
    }

    Status Resource::writeHeader(MemoryOutputStream & stream) const {
        if(getVersion()==NO_VERSION)
            return Status::WRONG_RESOURCE_VERSION;
        const char *str = typeid(*this).name();
        stream.writeASCII(str);
        stream.writeUInt(getVersion());
        return Status::OK;
    }

    Status Resource::readHeader(MemoryInputStream & stream) {
        const char *str = typeid(*this).name();
        size_t len = strlen(str);
        char *str2 = (char*)stream.readBytes(len+1);
        if(memcmp(str2, str, len+1)!=0)
            return Status::WRONG_RESOURCE_TYPE;
        if(stream.readUInt()!=getVersion()||getVersion()==NO_VERSION)
            return Status::WRONG_RESOURCE_VERSION;
        return Status::OK;
    }

    Status Resource::load(ResourceManager & resourceManager, unsigned long *bytesRead, LoadOption options) {
        File file(fileName);
        if(!file.Exists) {
            Logger::log(_T("'%s' doesn't exist\n"), fileName);
            return Status::FILE_DOESNT_EXIST;
        }

        Status result = file.read();
        if(result != Status::OK)
            return result;
        MemoryInputStream stream(file.Data, file.Size);
        result = load(resourceManager, stream);
        if(bytesRead!=nullptr)
            *bytesRead = stream.BytesRead;
        return result;
    }

    Status Resource::load(ResourceManager & resourceManager, const String & fileName, unsigned long *bytesRead, LoadOption options) {
        if(this->fileName.Length==0&&fileName.Length==0) {
            Logger::log(_T("Both resource's file name and the file name passed as parameter are empty\n"));
            return Status::INV_PARAM;
        } else if(fileName!=nullptr) {
            this->fileName = fileName;
        }

        return load(resourceManager, bytesRead);
    }

    Status Resource::load(ResourceManager & resourceManager, File & file, unsigned long *bytesRead, LoadOption options) {
        this->fileName = file.Name;
        if(!file.Exists)
            return Status::FILE_DOESNT_EXIST;
        Status result;
        if(file.Size==0) {
            result = file.read();
            if(result!=Status::OK)
                return result;
        }
        MemoryInputStream stream(file.Data, file.Size);
        result = load(resourceManager, stream);
        if(bytesRead!=nullptr)
            *bytesRead = stream.BytesRead;
        return result;
    }

    Status Resource::load(ResourceManager &resourceManager, MemoryInputStream &stream, LoadOption options) {
        Status result = loadInternal(resourceManager, stream, options);
        valid = result == Status::OK;
        return result;
    }

    Status Resource::save(ResourceManager &resourceManager, SaveOption options) const {
        File file(this->fileName);
        if(file.Exists&&options&SaveOption::SKIP_IF_EXISTS)
            return Status::OK;
        if(file.Exists&&!(options&SaveOption::OVERWRITE))
            return Status::FILE_EXISTS;
        MemoryOutputStream stream;
        Status result = save(resourceManager, stream, options);
        if(result!=Status::OK)
            return result;
        file.setData(stream.Data, stream.BytesWritten);
        return file.write();
    }

    Status Resource::save(ResourceManager &resourceManager, const String &fileName, SaveOption options) {
        if(this->fileName.Length==0&&fileName.Length==0) {
            Logger::log(_T("Both resource's file name and the file name passed as parameter are null\n"));
            return Status::INV_PARAM;
        } else if(fileName!=nullptr) {
            this->fileName = fileName;
        }

        File file(this->fileName);
        if(file.Exists&&options&SaveOption::SKIP_IF_EXISTS)
            return Status::OK;
        if(file.Exists&&!(options&SaveOption::OVERWRITE))
            return Status::FILE_EXISTS;

        return save(resourceManager);
    }

    Status Resource::save(ResourceManager &resourceManager, File & file, SaveOption options) const {
        if(file.Exists&&options&SaveOption::SKIP_IF_EXISTS)
            return Status::OK;
        if(file.Exists&&!(options&SaveOption::OVERWRITE))
            return Status::FILE_EXISTS;

        MemoryOutputStream stream;
        Status result = save(resourceManager, stream);
        if(result!=Status::OK)
            return result;
        file.setData(stream.Data, stream.BytesWritten);
        return Status::OK;
    }

    Status Resource::save(ResourceManager &resourceManager, MemoryOutputStream &stream, SaveOption options) const {
        return saveInternal(resourceManager, stream, options);
    }
}