#pragma once

namespace Ghurund {
    class ResourceFormat {
    private:
        const wchar_t* extension = nullptr;
        bool save, load;

        static const ResourceFormat* values[14];

    public:
        static const ResourceFormat& ENTITY,
            & SCENE,
            & HLSL, & SHADER,
            & JPG, & JPEG, & PNG,
            & SVG,
            & MATERIAL,
            & MODEL,
            & MESH, & OBJ,
            & WAV,
            & SCRIPT;

        ResourceFormat() {
            extension = nullptr;
            save = false;
            load = false;
        }

        ResourceFormat(const wchar_t* extension, bool save, bool load) {
            this->extension = extension;
            this->save = save;
            this->load = load;
        }

        ResourceFormat(const ResourceFormat& format) {
            this->extension = format.extension;
            this->save = format.save;
            this->load = format.load;
        }

        static const ResourceFormat** getValues() {
            return values;
        }

        static const size_t getValueCount() {
            return sizeof(values) / sizeof(values[0]);
        }

        const wchar_t* getExtension() const {
            return extension;
        }

        bool canSave() const {
            return save;
        }

        bool canLoad() const {
            return load;
        }
    };
}
