#include "Material.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"

namespace Ghurund {
    Status Material::loadInternal(ResourceManager &resourceManager, ResourceContext &context, MemoryInputStream &stream, LoadOption options) {
        Status result;
        shader = resourceManager.load<Ghurund::Shader>(context, stream.readUnicode(), &result, options);
        if(filterStatus(result, options)!=Status::OK)
            return result;

        size_t textureCount = stream.read<size_t>();
        for(size_t i = 0; i<textureCount; i++) {
            ASCIIString name = stream.readASCII();
            Texture *texture = (Texture*)resourceManager.load(context, stream, &result, options);
            if(filterStatus(result, options)!=Status::OK)
                return result;
            textures.set(name, texture);
            texture->release();
        }

        return result;
    }

    Status Material::saveInternal(ResourceManager &resourceManager, MemoryOutputStream &stream, SaveOption options) const {
        if(shader==nullptr) {
            Logger::log(_T("Shader cannot be empty\n"));
            return Status::INV_STATE;
        }

        stream.writeUnicode(shader->FileName);
        stream.write<size_t>(textures.Size);
        for(size_t i = 0; i<textures.Size; i++) {
            stream.writeASCII(textures.getKey(i));
            resourceManager.save(*textures.getValue(i), stream, options);
        }

        return Status::OK;
    }
}