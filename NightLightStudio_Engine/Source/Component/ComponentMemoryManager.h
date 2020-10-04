#pragma once


#include <map>
#include <vector>

#define BLOCK_SIZE_DEFAULT 164 // temp


///////////////////////////////////////////////////////////////////////////////////////////////////
// ComponentMemoryManager

class ComponentMemoryManager
{
public:

	typedef int ComponentType;
	struct ComponentTypeSettings
	{
		size_t elementSize;
		bool mustBeContiguous;
		int blockSize;
		bool doInitData;
		unsigned char initTo;
		// int initContainerSize , > 1

		ComponentTypeSettings() :
			elementSize(1),
			mustBeContiguous(false), // no new blocks
			blockSize(BLOCK_SIZE_DEFAULT),
			doInitData(true),
			initTo(0)
		{
		}

	};

private:
	struct ComponentMetaData
	{
		int currentSize;
		int currentMaxSize;
		ComponentTypeSettings settings;
		ComponentMetaData() :
			currentSize(0),
			currentMaxSize(-1)
		{
		}
	};
	std::map<ComponentType, ComponentMetaData> componentTypes;
	std::map<ComponentType, std::vector<char*>*> componentContainers;
	std::map<ComponentType, std::vector<char>*> componentContainerBits;

	ComponentMetaData* findMeta(ComponentType comT);

	std::vector<char*>* findContainer(ComponentType comT);

	std::vector<char>* findContainerBits(ComponentType comT);

	char* allocateBlock(size_t size);

	//void freeBlock(char* freeBlock)
	//{
	//	free(reinterpret_cast<void*>(freeBlock));
	//}

	bool getBitAt(ComponentType comT, int index);

	void setBitAt(ComponentType comT, int index, bool set);

public:
	ComponentType createNewComponentType(ComponentTypeSettings set);

	char* getElementAt(ComponentType comT, int index);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Iterator
public:
	class MemConIterator
	{
		friend class ComponentMemoryManager;

		int currentIndex; // index of item in container including free items
		int currentObjIndex; // index of item considering only occupied items

		int currentElementIndex; // index of item in container including free items
		char* currentElement;

		int currentBlockIndex; // index of mem block
		char* currentBlock;

		ComponentType currentT; // current component Type

		ComponentMetaData* meta; // ptr to current meta data
		std::vector<char*>* currentContainer; // ptr to current container
		std::vector<char>* currentContainerB; // ptr to current bit container

	public:
		// ctor
		MemConIterator() :
			currentIndex(0),
			currentObjIndex(0),
			currentElementIndex(0),
			currentElement(nullptr),
			currentBlockIndex(0),
			currentBlock(nullptr),
			currentT(-1),
			meta(nullptr),
			currentContainer(nullptr),
			currentContainerB(nullptr)
		{
		}

		MemConIterator(const MemConIterator& itr) = default;

		// copy
		MemConIterator& operator=(MemConIterator& itr) = default;
		MemConIterator& operator=(MemConIterator&& itr) = default;

		// comparison op w itrs
		bool operator==(MemConIterator& itr);

		bool operator!=(MemConIterator& itr);

		//bool operator<(Iterator& itr)
		//{
		//	return currentObjIndex < itr.currentObjIndex;
		//}

		//bool operator<=(Iterator& itr)
		//{
		//	return currentObjIndex <= itr.currentObjIndex;
		//}

		//bool operator>(Iterator& itr)
		//{
		//	return currentObjIndex > itr.currentObjIndex;
		//}

		//bool operator>=(Iterator& itr)
		//{
		//	return currentObjIndex >= itr.currentObjIndex;
		//}

		// other op
		char* operator*();

		MemConIterator& operator++();

		MemConIterator operator++(int); // post fix // slower

		int getCurrentIndex();

	private:
		// get the current element
		char* getElement();

		// check if the current element is free
		bool checkUnClear();

		// iterate to the next element (even if it is free)
		// returns the state of current element
		bool next();

		// iterate to the next unfree element
		void nextOccupied();

	};
	// Iterator END
	///////////////////////////////////////////////////////////////////////////////////////////////////

	MemConIterator end(ComponentType comT);

	MemConIterator begin(ComponentType comT);

	int insertIntoContainer(ComponentType comT, char* obj);

	void removeFromContainer(ComponentType comT, int index);

	void removeFromContainer(MemConIterator& itr);


	// I hope no reaks
	void freeAll();


private:
	void expandContainer(ComponentType comT);

	//void resetElement(ComponentType comT, int index)
	//{
	//}

	//char* setEmptyElement(ComponentType comT, int index)
	//{
	//}

	//void compressContainer()
	//{
	//}

};

// ComponentMemoryManager END
///////////////////////////////////////////////////////////////////////////////////////////////////
