#pragma once

#include "ResourceFormat.h"
#include "Status.h"
#include "core/Pointer.h"

#include <stdint.h>

namespace Ghurund {
    enum class LoadOption {
        DEFAULT = 0, DONT_WATCH = 1
    };

    LoadOption operator |(LoadOption lhs, LoadOption rhs);

    bool operator &(LoadOption lhs, LoadOption rhs);

    enum class SaveOption {
        DEFAULT = 0, OVERWRITE = 1, SKIP_IF_EXISTS = 2, FORMAT_JPG = 4, FORMAT_PNG = 8
    };

    SaveOption operator |(SaveOption lhs, SaveOption rhs);

    bool operator &(SaveOption lhs, SaveOption rhs);


    Status filterStatus(Status result, LoadOption option);
    Status filterStatus(Status result, SaveOption option);

    struct DataSize {
        uint64_t graphical, system;
    };

    class ResourceManager;
    class ResourceContext;
    class MemoryInputStream;
    class MemoryOutputStream;
    class File;
    class FilePath;
    class DirectoryPath;

    class Resource: public Pointer {
    private:
        bool valid = false;
        FilePath* path = nullptr;

        Status saveInternal(ResourceContext& context, File& file, SaveOption options) const;

    protected:
        DataSize dataSize;

        virtual Status loadInternal(ResourceContext& context, const DirectoryPath& workingDir, MemoryInputStream& stream, LoadOption options) = 0;
        virtual Status saveInternal(ResourceContext& context, const DirectoryPath& workingDir, MemoryOutputStream& stream, SaveOption options) const = 0;

        virtual unsigned int getVersion() const {
            return 0;
        }

        Status writeHeader(MemoryOutputStream& stream) const;
        Status readHeader(MemoryInputStream& stream);

        static const Ghurund::Type& GET_TYPE();

    public:
        Resource() = default;

        ~Resource();

        Status load(ResourceContext& context, size_t* bytesRead = nullptr, LoadOption options = LoadOption::DEFAULT);
        Status load(ResourceContext& context, const FilePath& path, size_t* bytesRead = nullptr, LoadOption options = LoadOption::DEFAULT);
        Status load(ResourceContext& context, File& file, size_t* bytesRead = nullptr, LoadOption options = LoadOption::DEFAULT);
        Status load(ResourceContext& context, const DirectoryPath& workingDir, MemoryInputStream& stream, LoadOption options = LoadOption::DEFAULT);

        Status save(ResourceContext& context, SaveOption options = SaveOption::DEFAULT) const;
        Status save(ResourceContext& context, const FilePath& path, SaveOption options = SaveOption::DEFAULT);

        // this method doesn't write the file contents to disk, remember to call File::write()
        Status save(ResourceContext& context, File& file, SaveOption options = SaveOption::DEFAULT);
        Status save(ResourceContext& context, const DirectoryPath& workingDir, MemoryOutputStream& stream, SaveOption options = SaveOption::DEFAULT) const;

        virtual void reload(ResourceContext& context) {
            valid = true;
            invalidate();
            addReference();
            load(context);
            release();
        }

        virtual void invalidate() {
            valid = false;
        }

        void setValid(bool valid) {
            this->valid = valid;
        }

        virtual bool isValid() const {
            return valid;
        }

        __declspec(property(get = isValid, put = setValid)) bool Valid;

        const FilePath* getPath() const {
            return path;
        }

        void setPath(const FilePath* path);

        __declspec(property(get = getPath, put = setPath)) FilePath* Path;
        /*
        const DataSize& getSize() const {
            return dataSize;
        }

        __declspec(property(get = getSize)) const DataSize& Size;*/

        inline static const Ghurund::Type& TYPE = GET_TYPE();

        virtual const Ghurund::Type& getType() const override {
            return TYPE;
        }
    };
}