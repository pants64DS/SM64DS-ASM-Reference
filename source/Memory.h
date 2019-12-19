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
	MemoryNode* firstAllocatedBlock;
	MemoryNode* lastAllocatedBlock;
	uint16_t unk34;
	uint16_t unk36;

	void* Allocate(unsigned size, unsigned align);			//Allocates memory by calling AllocateMemory and returns a ptr to it. Size needn't be a multiple by four since it gets aligned before the call.
	void* AllocateMemory(unsigned size, unsigned align);	//Tries to find an address to fit the allocation aligned at align bytes. If enough space is available, it returns a pointer to the newly allocated memory. If not, it returns a nullptr.
	bool Deallocate(void* ptr);								//Deallocates, returns 1 if successful
	unsigned Reallocate(void* ptr, unsigned newSize);		//Reallocates and returns the final node size. If newSize is less than the previous size, a part of ptr is freed; if newSize is greater than the previous size, ptr may change in case the new size doesn't fit into the old memory block.
	unsigned MemoryLeft();									//Returns the total number of free bytes
	unsigned MaxAllocatableSize(unsigned align);			//Returns the size of the biggest contiguous free memory block aligned with least alignment padding (if multiple have the same size)

};


//0x020A4D38
struct UnkStruct
{
	unsigned unk00;
	unsigned unk04;
	uint16_t unk08;
	uint16_t unk0a;
};

static unsigned Func_0203C8E8(unsigned heapTarget, unsigned size);


struct MemoryNode
{
	struct TargetInfo
	{
		unsigned start;
		unsigned end;

		TargetInfo(const MemoryNode& node);		//Creates a TargetInfo from the node's parameters
	};

	char magic[2];								//DU (allocated) or RF (free)
	uint16_t unk02;
	unsigned size;								//Size not including node
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

	static MemoryNode* LinkNode(MemoryNode* hpxeFLNodePair, MemoryNode* newNode, MemoryNode* prevNode);			//Link node on heap (corrects previous and next) and return a pointer to the new node
	static MemoryNode* UnlinkNode(MemoryNode* hpxeFLNodePair, MemoryNode* target);								//Unlink node from heap (corrects previous and next) and return a pointer to the node before the unlinked one
	static MemoryNode* CreateNode(MemoryNode::TargetInfo* targetInfo, char* nodeType);							//Sets up a new node at targetInfo.start, zeroes it and stores the block size in it. Returns a pointer to the newly created node.
	static void* AllocateNode(MemoryNode* hpxeFreeNodePair, MemoryNode* freeNode, unsigned address, unsigned size, unsigned arg4);	//Allocates size bytes at address (and zeroes it) inside freeNode, fixes node links and returns a ptr to the data of the new node after setup
	static unsigned SizeofInternal(void* ptr);																	//Returns the allocated size of a generic pointer to an allocated memory block
	static bool FreeNode(MemoryNode* hpxeFreeNodePair, MemoryNode::TargetInfo* targetInfo);						//Frees the node and merges free blocks. Returns 1 if deallocation has been successful, 0 otherwise.

};


//vtable at 0x02099DD8, ctor at 0x0203CA80
struct ExpandingHeap : public Heap		//internal name: mHeap::ExpHeap_t
{
	HPXE* hpxe;

	ExpandingHeap();
	virtual ~ExpandingHeap();

	virtual void* Allocate(unsigned size, int addressMask);												//Allocates size bytes with an alignment of addressMask
	virtual bool Deallocate(void* ptr);																	//Deallocates ptr from heap
	virtual unsigned Sizeof(void* ptr);																	//Returns the size of an allocated block
	virtual unsigned Reallocate(void* ptr, unsigned newSize);											//Reallocates the memory given by ptr with the size of newSize
	virtual unsigned MemoryLeft();																		//Returns the number of unallocated bytes
	virtual unsigned MaxAllocatableSize();																//Returns the size of the largest contiguous free memory block

};


//vtable at 0x02099D48, ctor at 0x0203CA54
struct SolidHeap : public Heap			//internal name: mHeap::SolidHeap_t
{
	SolidHeap();
	virtual ~SolidHeap();
};


#endif	// SM64DS_MEMORY_INCLUDED