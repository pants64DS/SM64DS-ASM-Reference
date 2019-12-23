#ifndef SM64DS_MEMORY_INCLUDED
#define SM64DS_MEMORY_INCLUDED


//WARNING: Only use these objects if you REALLY know what you're doing. Heap management in SM64DS is VERY unstable when using functions outside of their respective safe scopes.
//WARNING: You should NEVER touch HeapAllocator functions when hacking. Those are used internally to manage the memory and represent the low level management of the heap. 
//WANRING: It is also strictly prohibited to write to HeapAllocator members. Read access is safe. 
//HINT: HeapAllocator objects are only (and REALLY only) defined for documentation purposes on the heap's internals and to register the symbols to aid debugging.
//HINT: All functions dealing indirectly with the heap (Solid-/ExpandingHeap) are safe to use, but be careful with the correct addresses if you try to create a new heap.
//HINT: align is sometimes an integer. If align is positive, the heap is traversed forwards; if negative, the heap is traversed backwards. The abs of align determines the real memory alignment.


struct NestedHeapIterator
{
	HeapAllocator* first;						//Pointer to the first HeapAllocator
	HeapAllocator* last;						//Pointer to the last HeapAllocator
	uint16_t nestedHeaps;						//Number of nested heaps belonging to the iterator
	uint16_t nestedIterOffset;					//Offset for all nested heaps from the start of HeapAllocator to prev (most of the time 4)

	NestedHeapIterator(unsigned offset);
	HeapAllocator* NextHeap(HeapAllocator* allocator);		//Returns the next heap of allocator (iterator object should be the parent). If allocator is 0, the first nested heap is returned instead.
	HeapAllocator* RecursiveFindNestedHeap(void* ptr);		//Recursive function that returns either the last nested allocator containing ptr or a nullptr if the allocator owning the iterator is already the last nested allocator.
	void AddHeap(HeapAllocator* allocator);					//Adds allocator to the iterator.
	void AddFirstHeap(HeapAllocator* allocator);			//Adds allocator to the iterator as the first element.

	static NestedHeapIterator* FindNestedHeap(void* ptr);	//Returns the last nested allocator's iterator containing ptr, or the root iterator if there is no nested iterator yet. Calls FindNestedHeap on the root iterator.

};


struct HeapAllocator
{
	const char magic[4];						//HPXE for ExpandingHeapAllocator, HMRF for SolidHeapAllocator
	HeapAllocator* prev;						//In case two heap allocators reside on the same heap, they're linked with prev and next
	HeapAllocator* next;
	NestedHeapIterator nestedHeapIt;
	void* heapStart;
	void* heapEnd;
	unsigned flags;								//1: Fill with 0's if set; 2: Unused but also set if 1 is set

	extern static bool isRootHeapIterInitialized;			//True if the root heap iterator has been initialized
	extern static NestedHeapIterator rootHeapIterator;		//Iterator containing the root heap

	HeapAllocator(unsigned name, void* heapStart, void* heapEnd, unsigned flags);

};



struct ExpandingHeapAllocator : public HeapAllocator
{
	MemoryNode* firstFreeBlock;
	MemoryNode* lastFreeBlock;
	MemoryNode* firstAllocatedBlock;
	MemoryNode* lastAllocatedBlock;
	uint16_t unk34;
	uint16_t allocationMode;		//0: First fit (first one); 1: Best fit (smallest one)

	void* Allocate(unsigned size, int align);				//Allocates memory by calling Allocate<Direction> and returns a ptr to it. Size needn't be a multiple by four since it gets aligned before the call.
	void* AllocateForwards(unsigned size, unsigned align);	//Tries to find an address to fit the allocation aligned at align bytes. If enough space is available, it returns a pointer to the newly allocated memory. If not, it returns a nullptr. Allocation traversial is performed forwards.
	void* AllocateBackwards(unsigned size, unsigned align);	//Tries to find an address to fit the allocation aligned at align bytes. If enough space is available, it returns a pointer to the newly allocated memory. If not, it returns a nullptr. Allocation traversial is performed backwards.
	bool Deallocate(void* ptr);								//Deallocates, returns 1 if successful
	unsigned Reallocate(void* ptr, unsigned newSize);		//Reallocates and returns the final node size. If newSize is less than the previous size, a part of ptr is freed; if newSize is greater than the previous size, ptr may change in case the new size doesn't fit into the old memory block.
	unsigned MemoryLeft();									//Returns the total number of free bytes
	unsigned MaxAllocatableSize(int align);					//Returns the size of the biggest contiguous free memory block aligned with least alignment padding (if multiple have the same size)

	static MemoryNode* LinkNode(MemoryNode* nodePair, MemoryNode* newNode, MemoryNode* prevNode);			//Link node on heap (corrects previous and next) and return a pointer to the new node
	static MemoryNode* UnlinkNode(MemoryNode* nodePair, MemoryNode* target);								//Unlink node from heap (corrects previous and next) and return a pointer to the node before the unlinked one
	static MemoryNode* CreateNode(MemoryNode::TargetInfo* targetInfo, uint16_t nodeType);						//Sets up a new node at targetInfo.start, zeroes it and stores the block size in it. Returns a pointer to the newly created node.
	static void* AllocateNode(MemoryNode* freeNodePair, MemoryNode* freeNode, void* address, unsigned size, unsigned arg4);	//Allocates size bytes at address (and zeroes it) inside freeNode, fixes node links and returns a ptr to the data of the new node after setup
	static unsigned SizeofInternal(void* ptr);																//Returns the allocated size of a generic pointer to an allocated memory block
	static bool FreeNode(MemoryNode* freeNodePair, MemoryNode::TargetInfo* targetInfo);						//Frees the node and merges free blocks. Returns 1 if deallocation has been successful, 0 otherwise.

	ExpandingHeapAllocator(void* heapEnd, unsigned flags);

};


struct SolidHeapAllocator : public HeapAllocator 
{
	void* freeBlockBegin;
	void* freeBlockEnd;
	unsigned unk2c;

	void* Allocate(unsigned size, int align);
	unsigned Reallocate(void* ptr, unsigned newSize);
	unsigned MemoryLeft(int align);

	static void* AllocateForwards(void* hmrfFreeBlock, unsigned size, unsigned align);
	static void* AllocateBackwards(void* hmrfFreeBlock, unsigned size, unsigned align);

	SolidHeapAllocator(void* heapEnd, unsigned flags);

};




struct MemoryNode
{
	struct TargetInfo
	{
		void* start;
		void* end;

		TargetInfo(const MemoryNode& node);		//Creates a TargetInfo from the node's parameters
	};

	char magic[2];								//DU (allocated) or RF (free)
	uint16_t unk02;
	unsigned size;								//Size of data block
	MemoryNode* prev;
	MemoryNode* next;
};





//Abstract heap base class
//vtable at 0x02099D90, ctor at 0x0203CAAC
struct Heap								//internal name: mHeap::Heap_t
{
	void* heapStart;
	unsigned heapSize;					//Heap area containing allocator and allocated data
	Heap* rootHeap;
	unsigned unk10;
	
	extern static Heap* rootHeapPtr;
	extern static Heap* gameHeapPtr;

	static SolidHeap* CreateSolidHeap(unsigned size, Heap* root, int align);								//Allocates a SolidHeap on root with size size (except heap object members) and alignment/allocation direction align. Returns a pointer to the newly created heap.
	static ExpandingHeap* CreateExpandingHeap(unsigned size, Heap* root, int align);						//Allocates an ExpandingHeap on root with size size (except heap object members) and alignment/allocation direction align. Returns a pointer to the newly created heap.
	static SolidHeapAllocator* CreateSolidHeapAllocator(void* address, unsigned size, unsigned flags);
	static ExpandingHeapAllocator* CreateExpandingHeapAllocator(void* address, unsigned size, unsigned flags);
	static ExpandingHeap* CreateRootHeap(void* address, unsigned size);										//Creates the root heap (and therefore doesn't allocate it on another heap which doesn't exist at this point)

	Heap(void* start, unsigned size, Heap* root);
	virtual ~Heap();

	//0x08
	virtual void* Allocate(unsigned size, int align) = 0;													//Allocates size bytes with an alignment/allocation direction of align
	virtual bool Deallocate(void* ptr) = 0;																	//Deallocates ptr from heap
	//0x14
	virtual bool Intact() = 0;																				//Returns 1 if the heap represents a valid object. Not very reliable to check if the heap is broken like SM64DS does...
	//0x1C
	virtual unsigned Reallocate(void* ptr, unsigned newSize) = 0;											//Reallocates the memory given by ptr with the size of newSize
	virtual unsigned Sizeof(void* ptr) = 0;																	//Returns the size of an allocated block
	virtual unsigned MaxAllocationUnitSize() = 0;															//Returns the maximum size that is allocatable at once
	virtual unsigned MaxAllocatableSize() = 0;																//Returns the size of the largest contiguous free memory block
	virtual unsigned MemoryLeft() = 0;																		//Returns the number of unallocated bytes
	//0x34
	//0x38
	//0x3C

};


//Double linked list heap
//vtable at 0x02099DD8, ctor at 0x0203CA80
struct ExpandingHeap : public Heap		//internal name: mHeap::ExpHeap_t
{
	ExpandingHeapAllocator* hpxe;

	ExpandingHeap(void* start, unsigned size, Heap* root, ExpandingHeapAllocator* hpxe);
	virtual ~ExpandingHeap();

	virtual void* Allocate(unsigned size, int addressMask) override;
	virtual bool Deallocate(void* ptr) override;
	virtual unsigned Sizeof(void* ptr) override;
	virtual unsigned Reallocate(void* ptr, unsigned newSize) override;
	virtual unsigned MemoryLeft() override;
	virtual unsigned MaxAllocatableSize() override;
	virtual unsigned MaxAllocationUnitSize() override;						//Calls ExpandingHeapAllocator::MaxAllocatableSize()
	virtual bool Intact() override;

};


//Linear heap
//vtable at 0x02099D48, ctor at 0x0203CA54
struct SolidHeap : public Heap			//internal name: mHeap::SolidHeap_t
{
	SolidHeapAllocator* hmrf;

	SolidHeap(void* start, unsigned size, Heap* root, SolidHeapAllocator* hmrf);
	virtual ~SolidHeap();

	virtual void* Allocate(unsigned size, int addressMask) override;
	virtual bool Deallocate(void* ptr) override;							//Crashes
	virtual unsigned Sizeof(void* ptr) override;							//Crashes and returns -1
	virtual unsigned Reallocate(void* ptr, unsigned newSize) override;		//Only allowed if ptr was the last allocation
	virtual unsigned MemoryLeft() override;
	virtual unsigned MaxAllocatableSize() override;							//Calls SolidHeapAllocator::MemoryLeft()
	virtual unsigned MaxAllocationUnitSize() override;						//Calls SolidHeapAllocator::MemoryLeft()
	virtual bool Intact() override;

};


#endif	// SM64DS_MEMORY_INCLUDED