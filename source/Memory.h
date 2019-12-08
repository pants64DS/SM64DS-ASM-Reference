#ifndef SM64DS_MEMORY_INCLUDED
#define SM64DS_MEMORY_INCLUDED


struct HPXE
{
	const char magic[4];			//HPXE
	unsigned unk04;
	unsigned unk08;
	unsigned unk0C;
	unsigned unk10;
	uint16_t unk14;
	uint16_t unk16;
	unsigned hpxeEnd;
	unsigned heapEnd;
	unsigned align;
	unsigned unk24;
	unsigned unk28;
	unsigned unk2C;
	unsigned unk30;
	uint16_t unk34;
	uint16_t unk36;
};


//vtable at 0x02099D90, ctor at 0x0203CAAC
struct Heap								//internal name: mHeap::Heap_t
{
	HPXE* hpxePtr;
	unsigned maxAllocatedBytes;			//First 0x18 bytes excluded (until HPXE)
	Heap* previousHeap;
	unsigned unk10;

	Heap(unsigned a, unsigned b, unsigned c, unsigned unused);
	virtual ~Heap();

};


//vtable at 0x02099DD8, ctor at 0x0203CA80
struct ExpandingHeap : public Heap		//internal name: mHeap::ExpHeap_t
{
	HPXE* hpxePtr2;
	HPXE heapParameters;

	ExpandingHeap();
	virtual ~ExpandingHeap();
};

//0x020A0EA0 active heap ptr?
//special struct at +0x18


//vtable at 0x02099D48, ctor at 0x0203CA54
struct SolidHeap : public Heap			//internal name: mHeap::SolidHeap_t
{
	SolidHeap();
	virtual ~SolidHeap();
};


#endif	// SM64DS_MEMORY_INCLUDED