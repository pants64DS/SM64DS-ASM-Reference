#ifndef SM64DS_MEMORY_INCLUDED
#define SM64DS_MEMORY_INCLUDED



struct HPXE
{
	const char magic[4];			//HPXE
	unsigned unk04;
	unsigned unk08;
	HPXE* nestedHPXE;				//Pointer to nested heap HXPE
	HPXE* nestedHPXE2;				//Pointer to nested heap after the first one
	uint16_t unk14;
	uint16_t unk16;
	unsigned heapStart;
	unsigned heapEnd;
	unsigned unk20;					//Align?
	MemoryNode* firstFreeBlock;
	MemoryNode* lastFreeBlock;
	AllocationNode* firstAllocatedBlock;
	AllocationNode* lastAllocatedBlock;
	uint16_t unk34;
	uint16_t unk36;
};



struct AllocationNode
{
	unsigned unk00;
	unsigned size;
	AllocationNode* previous;
	AllocationNode* next;
};


struct MemoryNode
{
	unsigned unk00;
	unsigned size;
	MemoryNode* previous;
	MemoryNode* next;
};




//vtable at 0x02099D90, ctor at 0x0203CAAC
struct Heap								//internal name: mHeap::Heap_t
{
	unsigned memoryBegin;
	unsigned maxAllocatedBytes;			//Heap area containing HPXE and allocated data
	Heap* rootHeap;
	unsigned unk10;

	Heap(unsigned a, unsigned b, unsigned c, unsigned unused);
	virtual ~Heap();

};


//vtable at 0x02099DD8, ctor at 0x0203CA80
struct ExpandingHeap : public Heap		//internal name: mHeap::ExpHeap_t
{
	HPXE* hpxe;

	ExpandingHeap();
	virtual ~ExpandingHeap();

	virtual void* Allocate(unsigned size, unsigned addressMask);	//Allocates size bytes with an alignment of addressMask
	virtual bool Deallocate(void* ptr);								//Deallocates ptr from heap (0x10)

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