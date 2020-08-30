#ifndef SM64DS_SCENE_INCLUDED
#define SM64DS_SCENE_INCLUDED

#include "Actor.h"
#include "Model.h"

struct Scene
{
    // vtable is at 02091528
	virtual bool BeforeInitResources();
	virtual void AfterInitResources(unsigned vfSuccess);
	virtual bool BeforeCleanupResources();
	virtual void AfterCleanupResources(unsigned vfSuccess);
	virtual bool BeforeBehavior();
	virtual void AfterBehavior(unsigned vfSuccess);
	virtual bool BeforeRender();
	virtual void AfterRender(unsigned vfSuccess);
	virtual ~Scene();

    void ResetFadersAndSound(); // also sets the scene as the root ActorBase in the scene tree

    static void SetAndStopColorFader();
    static void SetFaders(FaderBrightness* fader);
    // Spawns a new scene if there is a scene to be spawned
    static void SpawnIfNecessary();
    static void PrepareToSpawnBoot();

    // The actorID should be a scene actor ID.
    // The scene's color fader is very not sophisticated,
    // so the only colors you can set are black and white.
    void StartSceneFade(unsigned actorID, unsigned param, uint16_t fadeColor);
    // The actorID should be a scene actor ID.
    // Returns true if the scene is a new one
    bool SetSceneToSpawn(unsigned actorID, unsigned param);
};

#endif