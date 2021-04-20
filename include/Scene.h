#ifndef SM64DS_SCENE_INCLUDED
#define SM64DS_SCENE_INCLUDED

#include "Actor.h"
#include "Model.h"
#include "Particle.h"
#include "Collision.h"

//vtable at 0x2092680
struct Scene : public ActorDerived		//internal name: dScene
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

//vtable at 0x02091528
struct SceneBoot : public Scene			//internal name: dScBoot
{
	unsigned unk50;
	unsigned unk54;
};

//vtable at 0x020943C4, ctor at 0x020352B4
struct SceneMB : public Scene			//internal name: dScMB
{
	//size 0x68
	//ColorFader?
};

struct Area
{
	TextureTransformer* texSRT;
	bool showing;
	uint8_t unk5;
	uint16_t unk6;
	unsigned unk8;
};

//vtable at 020921c0, constructor at 0202e088
struct Stage : public Scene				//internal name: dScStage, ActorID = 0x003
{
    Stage();
	virtual int  InitResources();
	virtual bool BeforeInitResources();
	virtual int  CleanupResources();
	virtual int  Behavior();
	virtual int  Render();
	virtual void OnPendingDestroy();
	virtual ~Stage();

	Particle::SysTracker particleSysTracker;
	Model model;
	Area areas[0x08];
	MeshCollider clsn;
	uint8_t fogTable[0x20];
	bool enableFog;
	uint8_t fogInfo;
	uint16_t fogOffset;
	uint16_t fogColor;
	uint16_t unk992;
	uint8_t unk994[0x20];
	unsigned unk9b4;
	unsigned unk9b8;
	Model* skyBox;
	unsigned unk9c0;
	unsigned unk9c4;
};

#endif