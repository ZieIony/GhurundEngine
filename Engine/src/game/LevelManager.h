#pragma once

#include "collection/List.h"
#include "Scene.h"
#include "graphics/RenderingBatch.h"
#include "ParameterManager.h"
#include "Level.h"

namespace Ghurund {
    class LevelManager:public EventDispatcher {
    private:
        Level *level;

    public:
        ~LevelManager() {
            if(level!=nullptr)
                level->onUninit();
        }

        void setLevel(Level *level) {
            if(this->level!=nullptr)
                this->level->onUninit();
            if(level!=nullptr)
                level->onInit();
            this->level = level;
        }

        Level *getLevel() {
            return level;
        }

        virtual bool dispatchKeyEvent(KeyEvent &event) override {
            if(level!=nullptr)
                return level->dispatchKeyEvent(event);
            return false;
        }

        virtual bool dispatchMouseButtonEvent(MouseButtonEvent &event) override {
            if(level!=nullptr)
                return level->dispatchMouseButtonEvent(event);
            return false;
        }

        virtual bool dispatchMouseMotionEvent(MouseMotionEvent &event) override {
            if(level!=nullptr)
                return level->dispatchMouseMotionEvent(event);
            return false;
        }

        virtual bool dispatchMouseWheelEvent(MouseWheelEvent &event) override {
            if(level!=nullptr)
                return level->dispatchMouseWheelEvent(event);
            return false;
        }

        void update() {
            if(level!=nullptr)
                level->onUpdate();
        }

        void draw(CommandList &commandList, ParameterManager &parameterManager) {
            if(level!=nullptr)
                level->draw(commandList, parameterManager);
        }
    };
}