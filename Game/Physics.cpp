#include "pch.h"

#include "Physics.h"

#include <PxPhysicsAPI.h>
#include <extensions/PxDefaultAllocator.h>
#include <common/windows/PxWindowsDelayLoadHook.h>
#include <cooking/PxCooking.h>

using namespace physx;

static PxFoundation* g_pPxFoundation;
static PxPhysics*    g_pPxPhysics;
static PxCooking*    g_pPxCooking;
static PxScene*      g_pPxScene = nullptr; 
static PxMaterial*   g_pPxDefaultMaterial = nullptr;

class MyPxErrorCallback : public PxErrorCallback
{
    public:
        virtual void reportError(PxErrorCode::Enum code, char const* message, char const* file, int line) override
        {
            if (code & (PxErrorCode::eABORT | PxErrorCode::eINVALID_OPERATION |
                PxErrorCode::eINVALID_PARAMETER | PxErrorCode::eOUT_OF_MEMORY))
            {
                Logger::Get()->critical("[PhysX Critical Error] File: %s Line: %i Message: %s", file, line, message);
            }
            else if (code & PxErrorCode::eINTERNAL_ERROR)
            {
                Logger::Get()->error("[PhysX Error] File: %s Line: %i Message: %s", file, line, message);
            }
            else if (code & PxErrorCode::ePERF_WARNING)
            {
                Logger::Get()->warn("[PhysX Warning] File: %s Line: %i Message: %s", file, line, message);
            }
            else if (code & PxErrorCode::eDEBUG_WARNING)
            {
                Logger::Get()->warn("[PhysX Warning] File: %s Line: %i Message: %s", file, line, message);
            }
            else if (code & PxErrorCode::eDEBUG_INFO)
            {
                Logger::Get()->info("[PhysX Info] File: %s Line: %i Message: %s", file, line, message);
            }
        }
};

class MyPxSimulationEventCallback : public PxSimulationEventCallback
{
    public:
        virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
        virtual void onWake(PxActor** actors, PxU32 count) override {}
        virtual void onSleep(PxActor** actors, PxU32 count) override {}
        virtual void onContact(PxContactPairHeader const& pairHeader, PxContactPair const* pairs, PxU32 nbPairs) override {}
        virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}
        virtual void onAdvance(PxRigidBody const* const* bodyBuffer, PxTransform const* poseBuffer, PxU32 const count) override {}

};

static MyPxSimulationEventCallback g_PxSimulationCallback;

Physics::Physics()
{
}

void Physics::Initialize()
{
    static MyPxErrorCallback px_error_callback;
    static PxDefaultAllocator px_allocator_callback;

    PxTolerancesScale tolerances_scale;

    {
        g_pPxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, px_allocator_callback, px_error_callback);
        if (!g_pPxFoundation)
        {
            Logger::Get()->critical("Failed to initialize PhysX foundation!");
        }
    }

    {
        bool const record_mem_allocs = false;
        g_pPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_pPxFoundation, tolerances_scale, record_mem_allocs);
        if (!g_pPxPhysics)
        {
            Logger::Get()->critical("Failed to initialize PhysX physics object!");
        }
    }

    {
        g_pPxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *g_pPxFoundation, PxCookingParams(tolerances_scale));
        if (!g_pPxCooking)
        {
            Logger::Get()->critical("FAILED to initialize PhysX cooking object!");
        }

        PxCookingParams params(tolerances_scale);
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
        g_pPxCooking->setParams(params);
    }

    {
        PxInitExtensions(*g_pPxPhysics, nullptr);
    }

    {
        PxSceneDesc scene_desc(tolerances_scale);
        scene_desc.gravity = { 0.0f, -9.8f, 0.0f };
        scene_desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(4);
        scene_desc.filterShader = PxDefaultSimulationFilterShader;

        g_pPxScene = g_pPxPhysics->createScene(scene_desc);

        g_pPxScene->setSimulationEventCallback(&g_PxSimulationCallback);

        auto& m = DEFAULT_MATERIAL;
        g_pPxDefaultMaterial = g_pPxPhysics->createMaterial(m.static_friction, m.dynamic_friction, m.restitution);
    }
}

void Physics::Shutdown()
{
    g_pPxDefaultMaterial->release();
    g_pPxScene->release();
    PxCloseExtensions();
    g_pPxCooking->release();
    g_pPxPhysics->release();
    g_pPxFoundation->release();
}
