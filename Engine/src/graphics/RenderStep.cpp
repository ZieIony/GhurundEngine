#include "RenderStep.h"

namespace Ghurund {
    void RenderStep::cull() {
        XMMATRIX view = XMLoadFloat4x4(camera->View);
        XMVECTOR determinant;
        XMMATRIX viewInv = XMMatrixInverse(&determinant, view);
        XMMATRIX proj = XMLoadFloat4x4(camera->Projection);
        BoundingFrustum frustum(proj);
        frustum.Transform(frustum, viewInv);
        for (size_t i = 0; i < models.Size; i++)
            models[i]->cull(frustum);
    }

    Model* RenderStep::pick(XMINT2 & mousePos) {
        XMFLOAT3 pos, dir;
        float dist, closestDist;
        camera->calcMouseRay(mousePos, pos, dir);

        GlobalEntity<Model>* picked = nullptr;
        for (size_t i = 0; i < models.Size; i++) {
            GlobalEntity<Model>* model = models[i];
            if (!model->Visible || !model->Entity.Selectable)
                continue;
            if (model->intersects(pos, dir, dist) && (picked == nullptr || dist < closestDist)) {
                picked = model;
                closestDist = dist;
            }
        }

        return picked != nullptr ? &picked->Entity : nullptr;
    }

    void RenderStep::draw(Graphics & graphics, CommandList & commandList, RenderingStatistics & stats) {
        models.deleteItems();

        for (size_t i = 0; i < entities.Size; i++) {
            Entity* entity = entities[i];
            XMFLOAT4X4 identity;
            XMStoreFloat4x4(&identity, XMMatrixIdentity());
            entity->flatten(*this, identity);
        }

        camera->updateParameters();
        cull();

        for (size_t i = 0; i < models.Size; i++) {
            GlobalEntity<Model>* entity = models[i];
            if (!entity->Visible) {
                stats.modelsCulled++;
                continue;
            }

            Ghurund::Material* overrideMaterial = material;
            if (!entity->Entity.Valid) {
                if (entity->Entity.Mesh != nullptr && entity->Entity.Mesh->Valid
                    && entity->Entity.Material != nullptr && !entity->Entity.Material->Valid
                    && invalidMaterial != nullptr) {
                    overrideMaterial = invalidMaterial;
                } else {
                    continue;
                }
            }

            parameterWorld->setValue(&entity->Transformation);
            XMMATRIX world = XMLoadFloat4x4(&entity->Transformation);
            XMFLOAT4X4 worldIT;
            XMStoreFloat4x4(&worldIT, XMMatrixTranspose(XMMatrixInverse(nullptr, world)));
            parameterWorldIT->setValue(&worldIT);

            if (overrideMaterial != nullptr) {
                Ghurund::Material* modelMaterial = entity->Entity.Material;
                modelMaterial->addReference();
                entity->Entity.Material = overrideMaterial;
                entity->Entity.updateParameters();
                entity->Entity.draw(graphics, commandList, stats);
                entity->Entity.Material = modelMaterial;
                modelMaterial->release();
            } else {
                entity->Entity.updateParameters();
                entity->Entity.draw(graphics, commandList, stats);
            }
        }
    }
}