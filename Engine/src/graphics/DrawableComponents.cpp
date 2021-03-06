#include "ghpch.h"
#include "DrawableComponents.h"

#include "graphics/Materials.h"
#include "core/SharedPointer.h"
#include "graphics/mesh/CubeMesh.h"
#include "graphics/mesh/SphereMesh.h"

namespace Ghurund {
    DrawableComponent* DrawableComponents::makeCube(ResourceContext& context, Material& material) {
        SharedPointer<Mesh> mesh = ghnew CubeMesh();
        mesh->init(context.Graphics, context.CommandList);
        DrawableComponent* model = ghnew DrawableComponent();
		model->Mesh = mesh;
		model->Material = &material;
        model->initParameters(context.ParameterManager);
        model->Valid = true;

        return model;
    }

    DrawableComponent* DrawableComponents::makePlane(ResourceContext& context, Material& material, unsigned int detail) {
        SharedPointer<Mesh> mesh = ghnew PlaneMesh();
        mesh->init(context.Graphics, context.CommandList, detail);
		DrawableComponent* model = ghnew DrawableComponent();
		model->Mesh = mesh;
		model->Material = &material;
		model->initParameters(context.ParameterManager);
        model->Valid = true;

        return model;
    }

    DrawableComponent* DrawableComponents::makeSphere(ResourceContext& context, Material& material) {
        SharedPointer<Mesh> mesh = ghnew SphereMesh();
        mesh->init(context.Graphics, context.CommandList, 3);
		DrawableComponent* model = ghnew DrawableComponent();
		model->Mesh = mesh;
		model->Material = &material;
		model->initParameters(context.ParameterManager);
        model->Valid = true;

        return model;
    }

    DrawableComponent* DrawableComponents::makeCone(ResourceContext& context, Material& material) {
        SharedPointer<Mesh> mesh = ghnew ConeMesh();
        mesh->init(context.Graphics, context.CommandList);
		DrawableComponent* model = ghnew DrawableComponent();
		model->Mesh = mesh;
		model->Material = &material;
		model->initParameters(context.ParameterManager);
        model->Valid = true;

        return model;
    }

    DrawableComponent* DrawableComponents::makeQuad(ResourceContext& context, Material& material) {
        SharedPointer<Mesh> mesh = ghnew QuadMesh();
        mesh->init(context.Graphics, context.CommandList);
		DrawableComponent* model = ghnew DrawableComponent();
		model->Mesh = mesh;
		model->Material = &material;
		model->initParameters(context.ParameterManager);
        model->Valid = true;

        return model;
    }
}