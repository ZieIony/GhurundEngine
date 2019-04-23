#include "application/Application.h"
#include "audio/Sound.h"
#include "game/entity/Scene.h"
#include "game/entity/Models.h"
#include "graphics/Materials.h"
#include "game/CameraController.h"
#include "game/LevelManager.h"
#include "game/entity/Scenes.h"
#include "script/Scripts.h"

using namespace Ghurund;
using namespace DirectX;

class TestLevel:public Level {
private:
    Camera* camera = nullptr;
    CameraController* cameraController = nullptr;
    Application& app;
    Material* overrideMaterial = nullptr, * wireframeMaterial = nullptr, * checkerMaterial = nullptr;

    RenderStep editorStep, sceneStep;

public:
    TestLevel(Application& app):app(app) {}

    virtual bool onMouseButtonEvent(MouseButtonEvent& event) override {
        if (event.Action == MouseAction::DOWN) {
            Model* model = sceneStep.pick(app.Input.MousePos);
            if (model != nullptr)
                return true;
        }
        return cameraController->dispatchMouseButtonEvent(event);
    }

    virtual bool onMouseMouseMotionEvent(MouseMotionEvent& event) override {
        return cameraController->dispatchMouseMotionEvent(event);
    }

    virtual bool onMouseWheelEvent(MouseWheelEvent& event) override {
        return cameraController->dispatchMouseWheelEvent(event);
    }

    virtual bool onKeyEvent(KeyEvent& event) override {
        if (event.Action == KeyAction::DOWN && event.Key == 'W') {
            setPointer(overrideMaterial, wireframeMaterial);
        } else if (event.Action == KeyAction::UP && event.Key == 'W') {
            safeRelease(overrideMaterial);
        } else if (event.Action == KeyAction::DOWN && event.Key == 'C') {
            setPointer(overrideMaterial, checkerMaterial);
        } else if (event.Action == KeyAction::UP && event.Key == 'C') {
            safeRelease(overrideMaterial);
        } else if (event.Action == KeyAction::DOWN && event.Key == VK_ESCAPE) {
            PostQuitMessage(0);
            return true;
        }
        return false;
    }

    virtual void onInit() override {
        camera = ghnew Ghurund::Camera();
        camera->setPositionTargetUp({0, 50, -500}, {0, 50, 0});

        cameraController = ghnew CameraController(*camera, &app.Window);

        wireframeMaterial = Materials::makeWireframe(app.ResourceManager, app.ResourceContext);
        checkerMaterial = Materials::makeChecker(app.ResourceManager, app.ResourceContext);

        File sceneFile("test/test.scene");
        if (sceneFile.Exists) {
            app.ResourceManager.loadAsync<Ghurund::Scene>(app.AsyncResourceContext, "test/test.scene", [&](Ghurund::Scene * scene, Status result) {
                if (result != Status::OK)
                    return;
                scene->initParameters(app.ParameterManager);
                sceneStep.Entities.add(scene);
                scene->release();
                });
        } else {
            ScopedPointer<Scene> scene = makeScene();
            sceneStep.Entities.add(scene);

            Status result = scene->save(app.ResourceManager, app.ResourceContext, "test/test.scene", SaveOption::SKIP_IF_EXISTS);
            if (result != Status::OK)
                Logger::log(LogType::WARNING, _T("failed to save scene\n"));
        }

        editorStep.Camera = camera;
        ScopedPointer<Scene> editorScene = Scenes::makeEditor(app.ResourceManager, app.ResourceContext);
        editorStep.Entities.add(editorScene);
        editorStep.initParameters(app.ParameterManager);
        app.Renderer.Steps.add(&editorStep);

        sceneStep.Camera = camera;
        ScopedPointer<Material> invalidMaterial = Materials::makeInvalid(app.ResourceManager, app.ResourceContext);
        sceneStep.InvalidMaterial = invalidMaterial;
        sceneStep.initParameters(app.ParameterManager);
        app.Renderer.Steps.add(&sceneStep);

        const char* sourceCode = "void main(Camera &camera){camera.setOrbit(timer.getTime(),cos(timer.getTime()/5.0f)*3.0f+30);}";
        ScopedPointer<Script> script = Scripts::make(camera, sourceCode);

        app.ScriptEngine.Scripts.add(script);
    }

    Scene* makeScene() {
        ScopedPointer<Model> lamborghini;
        {
            ScopedPointer<Mesh> mesh;
            File file("test/obj/lamborghini/Lamborghini_Aventador.mesh");
            if (file.Exists) {
                mesh = app.ResourceManager.load<Mesh>(app.ResourceContext, file);
            } else {
                mesh = app.ResourceManager.load<Mesh>(app.ResourceContext, "test/obj/lamborghini/Lamborghini_Aventador.obj");
                if (mesh != nullptr)
                    mesh->save(app.ResourceManager, app.ResourceContext, file);
            }

            ScopedPointer<Texture> texture = Textures::makeFromImage(app.ResourceManager, app.ResourceContext, "test/obj/lamborghini/Lamborginhi Aventador_diffuse.jpeg");
            if (texture != nullptr && mesh != nullptr) {
                ScopedPointer<Material> material = Materials::makeBasicLight(app.ResourceManager, app.ResourceContext, *texture);

                lamborghini = ghnew Model(mesh, material);
                lamborghini->Name = "lamborghini";
                lamborghini->Valid = true;
            }
        }

        ScopedPointer<Model> cone;
        {
            ScopedPointer<Material> material = Materials::makeNormals(app.ResourceManager, app.ResourceContext);
            cone = Models::makeCone(app.ResourceContext, *material);

            cone->Name = "cone";
            cone->Position = {100, 100, 100};
            cone->Scale = {50, 100, 50};
        }

        ScopedPointer<Model> selection = Models::makeSelection(app.ResourceManager, app.ResourceContext, *lamborghini);

        return ghnew Scene({lamborghini, cone, selection});
    }

    virtual void onUpdate() override {
        camera->ScreenSize = app.Window.Size;
    }

    virtual void onUninit() override {
        camera->release();
        wireframeMaterial->release();
        checkerMaterial->release();
        safeRelease(overrideMaterial);
        delete cameraController;
    }
};