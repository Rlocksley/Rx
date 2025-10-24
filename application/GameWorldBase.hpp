 #pragma once

#include "Global.hpp"
#include "Command.hpp"
#include "RenderPass.hpp"
#include "Mutex.hpp"
#include "flecs.h"
#include "Level.hpp"

namespace Rx{

    struct RenderRunning{};
    struct ShouldBeRendered{};
    struct ShouldBeUpdated{};

    class Application;
    class GameWorldBase {
    public:
        GameWorldBase(Application& app, flecs::world& world) : world(world), app(app) {
        }

        flecs::world& getWorld() {
            return world;
        }

    public:
        virtual void loadGlobal();
        
    protected:
        Application& app;

        flecs::world& world;
        flecs::entity game;

        //render entities
        flecs::entity postUpdate;
        flecs::entity preRender;
        flecs::entity onRecordBegin;
        flecs::entity onRecordBarrier1;
        flecs::entity onRecordComp;
        flecs::entity onRecordShadow;
        flecs::entity onRecordRenderPassBegin;
        flecs::entity onRecordBarrier2;
        flecs::entity onRecordRender;
        flecs::entity onRecordEnd;


        flecs::entity colorMeshArrayInstanceRelation;
        flecs::entity instancedColorMeshRelation;
        flecs::entity textureModelInstanceRelation;
        flecs::entity IsSkeletonOf;
        flecs::entity IsSkeletonModelInstanceOf;
        flecs::entity IsAnimationOf;
        flecs::entity IsAnimationInstanceOf;
        flecs::entity IsRootNodeOf;
        flecs::entity IsRootNodeInstanceOf;
        flecs::entity IsNodeOf;
        flecs::entity IsChildNodeOf;
        flecs::entity IsChildNodeInstanceOf;
        flecs::entity IsNodeOfChild;


    private:
        void registerComponentsBase();

        void registerObserversBase();

        void registerGraphicsBase();
    };
}