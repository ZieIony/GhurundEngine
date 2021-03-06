#pragma once

#include "ecs/EntityList.h"

namespace Ghurund {

    class EntityGroup:public Entity {
    protected:
        EntityList entities;
        mutable DirectX::BoundingBox boundingBox;

        virtual Status loadInternal(ResourceContext& context, const DirectoryPath& workingDir, MemoryInputStream& stream, LoadOption options) override;
        virtual Status saveInternal(ResourceContext& context, const DirectoryPath& workingDir, MemoryOutputStream& stream, SaveOption options) const override;

    public:

        EntityGroup():entities(*this) {}

        EntityGroup(const std::initializer_list<Entity*> list):entities(*this) {
            for (auto it = list.begin(); it != list.end(); ++it)
                entities.add(*it);
        }

        virtual void initParameters(ParameterManager& parameterManager) override {
            for (Entity* e : entities)
                e->initParameters(parameterManager);
        }

        virtual void updateParameters() override {
            for (Entity* e : entities)
                e->updateParameters();
        }

        virtual DirectX::BoundingBox* getBoundingBox() const override {
            bool init = false;
            for (Entity* e : entities) {
                if (e->BoundingBox == nullptr)
                    continue;
                if (init) {
                    DirectX::BoundingBox b;
                    e->BoundingBox->Transform(b, XMLoadFloat4x4(&e->getTransformation()));
                    boundingBox.CreateMerged(boundingBox, boundingBox, b);
                } else {
                    e->BoundingBox->Transform(boundingBox, XMLoadFloat4x4(&e->getTransformation()));
                    init = true;
                }
            }
            return &boundingBox;
        }

        EntityList& getEntities() {
            return entities;
        }

        __declspec(property(get = getEntities)) EntityList& Entities;

        Entity* findEntity(String& name) {
            for (Entity* entity : entities) {
                if (entity->Name == name)
                    return entity;
            }

            return nullptr;
        }

        List<Entity*>* findEntities(String& name) {
            List<Entity*>* list = ghnew List<Entity*>();
            for (Entity* entity : entities) {
                if (entity->Name == name)
                    list->add(entity);
            }

            return list;
        }

        Entity* findEntity(Ghurund::Type& type) {
            for (Entity* entity : entities) {
                if (entity->Type == type)
                    return entity;
            }

            return nullptr;
        }

        List<Entity*>* findEntities(Ghurund::Type& type) {
            List<Entity*>* list = ghnew List<Entity*>();
            for (Entity* entity : entities) {
                if (entity->Type == type)
                    list->add(entity);
            }

            return list;
        }

        virtual void queueDraw(RenderStep& step, XMFLOAT4X4& transformation) override {
			if (!Visible)
				return;

			XMFLOAT4X4 localTransformation = getTransformation();
			XMFLOAT4X4 dest;
			XMStoreFloat4x4(&dest, XMLoadFloat4x4(&transformation) * XMLoadFloat4x4(&localTransformation));

            for (size_t i = 0; i < Entities.Size; i++) {
                Entity* entity = Entities[i];
                entity->queueDraw(step, dest);
            }
        }

    };
}