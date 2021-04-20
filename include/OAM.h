#pragma once
#include "SM64DS_Common.h"

namespace OAM {

	//----------------------------------------------------------------------------------------------------------------
	// internal use
	//----------------------------------------------------------------------------------------------------------------


	void render(BOOL sub, GXOamAttr* data, int x, int y, int palette, int priority, MtxFx22* affineMtx);

	void render(BOOL sub, GXOamAttr* data, int x, int y, int palette, int priority, fx32 scale, int rotation);

	void render(BOOL sub, GXOamAttr* data, int x, int y, int palette, int priority, fx32 scaleX, fx32 scaleY, int rotation, int mode);


	u8 getObjWidth(int shape, int size);

	u8 getObjHeight(int shape, int size);


	void loadAffineParams(GXOamAttr* data, int* affineSet, MtxFx22* affineMtx);


	// indexing: [shape][size] (shape 4 is prohibited)
	u8 objWidth[3][4];
	u8 objHeight[3][4];


	bool subOAMDisabled;

	int curAffineSetMain;
	int curAffineSetSub;

	int objCountMain;
	int objCountSub;

	GXOamAttr bufferMain[128];
	GXOamAttr bufferSub[128];


	//----------------------------------------------------------------------------------------------------------------
	// public functions
	//----------------------------------------------------------------------------------------------------------------


	// Resets the internal buffer
	void reset();

	// Flushes the cache on the internal buffer
	void flush();

	// Loads the internal buffer to the OAM
	void load();


	// Inserts an object in the internal buffer
	void render(GXOamAttr* data, int x, int y);

	// Inserts an object in the internal buffer
	void render(GXOamAttr* data, int x, int y, int palette, int priority);


	// Enables the sub engine OAM
	void enableSubOAM();
	// Disables the sub engine OAM
	inline void disableSubOAM() {
		subOAMDisabled = true;
	}

	// Returns the internal buffer for the main engine
	inline GXOamAttr* getMainBuffer() {
		return bufferMain;
	}
	// Returns the internal buffer for the sub engine
	inline GXOamAttr* getSubBuffer() {
		return bufferSub;
	}

	// Returns the number of used objects for the main engine
	inline int getObjCountMain() {
		return objCountMain;
	}
	// Returns the number of used objects for the sub engine
	inline int getObjCountSub() {
		return objCountSub;
	}

	// Returns the current affine set for the main engine
	inline int getCurrentAffineSetMain() {
		return curAffineSetMain;
	}
	// Returns the current affine set for the sub engine
	inline int getCurrentAffineSetSub() {
		return curAffineSetSub;
	}

}
