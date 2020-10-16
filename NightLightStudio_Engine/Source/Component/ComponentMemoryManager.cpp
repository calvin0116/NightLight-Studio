

#include "ComponentMemoryManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// ComponentMemoryManager


ComponentMemoryManager::ComponentMetaData* ComponentMemoryManager::findMeta(ComponentMemoryManager::ComponentType comT)
{
	// find settings
	auto foundT = componentTypes.find(comT);
	if (foundT == componentTypes.end())
	{
		// not found
		throw;
	}
	// found
	return &(*foundT).second;
}

std::vector<char*>* ComponentMemoryManager::findContainer(ComponentMemoryManager::ComponentType comT)
{
	//find container
	auto foundC = componentContainers.find(comT);
	if (foundC == componentContainers.end())
	{
		// not found
		throw;
	}
	// found
	return (*foundC).second;
}

std::vector<char>* ComponentMemoryManager::findContainerBits(ComponentMemoryManager::ComponentType comT)
{
	// find container bits
	auto foundB = componentContainerBits.find(comT);
	if (foundB == componentContainerBits.end())
	{
		// not found
		throw;
	}
	// found
	return (*foundB).second;
}

char* ComponentMemoryManager::allocateBlock(size_t size)
{
	void* newBlock = malloc(size);
	return reinterpret_cast<char*>(newBlock);
}

ComponentMemoryManager::ComponentType ComponentMemoryManager::createNewComponentType(ComponentMemoryManager::ComponentTypeSettings set)
{
	// define new component type
	ComponentType newComponentType = (int)componentTypes.size();
	while (componentTypes.find(newComponentType) != componentTypes.end())
	{
		// found
		newComponentType++;
	}
	ComponentMetaData meta;
	meta.settings = set;
	meta.currentMaxSize = set.blockSize;
	componentTypes.insert(std::pair<ComponentType, ComponentMetaData>(newComponentType, meta));

	// create new component container
	std::vector<char*>* newContainer = new std::vector<char*>;
	char* newBlock = allocateBlock(set.elementSize * set.blockSize);
	if (set.doInitData)
	{
		memset(newBlock, set.initTo, set.elementSize * set.blockSize);
	}
	newContainer->push_back(newBlock);
	componentContainers.insert(std::pair<ComponentType, std::vector<char*>*>(newComponentType, newContainer));

	// init container bits
	std::vector<char>* newBitContainer = new std::vector<char>;
	int n = set.blockSize / 8 /*size of char*/;
	++n;
	for (int i = 0; i < n; ++i)
	{
		char newBits = 0;
		newBitContainer->push_back(newBits);
	}
	componentContainerBits.insert(std::pair<ComponentType, std::vector<char>*>(newComponentType, newBitContainer));

	return newComponentType;
}

char* ComponentMemoryManager::getElementAt(ComponentMemoryManager::ComponentType comT, int index)
{
	ComponentMetaData* meta = findMeta(comT);
	std::vector<char*>* container = findContainer(comT);
	ComponentTypeSettings settings = meta->settings;

	int blockindex = index / settings.blockSize;
	int elementindex = index % settings.blockSize;

	char* con = (*container)[blockindex];

	char* elementFound = con + settings.elementSize * (elementindex);

	return elementFound;
}

bool ComponentMemoryManager::getBitAt(ComponentMemoryManager::ComponentType comT, int index)
{
	std::vector<char>* containerB = findContainerBits(comT);

	int bitindex = index / 8;
	int bitelementindex = index % 8;

	char bt = (*containerB)[bitindex];

	return (bt >> bitelementindex) & 0x1;
}

void ComponentMemoryManager::setBitAt(ComponentMemoryManager::ComponentType comT, int index, bool set)
{
	std::vector<char>* containerB = findContainerBits(comT);

	int bitindex = index / 8;
	int bitelementindex = index % 8;

	char* bt = &(*containerB)[bitindex];

	int n = set; // !

	*bt ^= (-n ^ *bt) & (1 << bitelementindex);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Iterator

bool ComponentMemoryManager::MemConIterator::operator==(ComponentMemoryManager::MemConIterator& itr)
{
	return currentObjIndex == itr.currentObjIndex;
}

bool ComponentMemoryManager::MemConIterator::operator!=(ComponentMemoryManager::MemConIterator& itr)
{
	return currentObjIndex != itr.currentObjIndex;
}

char* ComponentMemoryManager::MemConIterator::operator*()
{
	return getElement();
}

ComponentMemoryManager::MemConIterator& ComponentMemoryManager::MemConIterator::operator++()
{
	nextOccupied();
	return *this;
}

ComponentMemoryManager::MemConIterator ComponentMemoryManager::MemConIterator::operator++(int) // post fix // slower
{
	MemConIterator newItr = *this;
	nextOccupied();
	return newItr;
}

int ComponentMemoryManager::MemConIterator::getCurrentIndex()
{
	return currentIndex;
}

int ComponentMemoryManager::MemConIterator::getCurrentObjIndex()
{
	return currentObjIndex;
}

char* ComponentMemoryManager::MemConIterator::getElement()
{
	return currentElement;
}

bool ComponentMemoryManager::MemConIterator::checkUnClear()
{
	int bitindex = currentIndex / 8;
	int bitelementindex = currentIndex % 8;

	char bt = (*currentContainerB)[bitindex];

	return (bt >> bitelementindex) & 0x1;
}

bool ComponentMemoryManager::MemConIterator::next()
{

	if ((currentIndex + 1) >= meta->currentMaxSize)
	{
		// reached the end of the container
		++currentObjIndex; // one element after the last // its a virtual element
		return true; // return found element to end loop
	}

	++currentIndex;
	++currentElementIndex;

	if ((currentElementIndex + 1) > meta->settings.blockSize)
	{
		// itr to next block
		++currentBlockIndex;
		currentBlock = (*currentContainer)[currentBlockIndex];
		currentElement = currentBlock;
		currentElementIndex = 0;
	}
	else
	{
		currentElement += meta->settings.elementSize;
	}

	bool notclear = checkUnClear();
	if (notclear)
		++currentObjIndex;

	return notclear;
}

void ComponentMemoryManager::MemConIterator::nextOccupied()
{
	bool isClear = !next();
	while (isClear)
	{
		isClear = !next();
	}
}

// Iterator END
///////////////////////////////////////////////////////////////////////////////////////////////////


ComponentMemoryManager::MemConIterator ComponentMemoryManager::end(ComponentMemoryManager::ComponentType comT)
{
	MemConIterator newItr;
	newItr.currentT = comT;

	ComponentMetaData* meta = findMeta(comT);
	newItr.meta = meta;

	std::vector<char*>* container = findContainer(comT);
	newItr.currentContainer = container;

	std::vector<char>* containerB = findContainerBits(comT);
	newItr.currentContainerB = containerB;

	// I hope this is correct
	newItr.currentIndex = meta->currentMaxSize - 1;
	newItr.currentObjIndex = meta->currentSize /*- 1*/;
	newItr.currentElementIndex = meta->settings.blockSize - 1;
	newItr.currentBlockIndex = (meta->currentMaxSize / meta->settings.blockSize) - 1;
	newItr.currentBlock = (*newItr.currentContainer)[newItr.currentBlockIndex];
	newItr.currentElement = newItr.currentBlock + meta->settings.elementSize * (meta->settings.elementSize - 1);

	return newItr;
}

ComponentMemoryManager::MemConIterator ComponentMemoryManager::begin(ComponentMemoryManager::ComponentType comT)
{
	MemConIterator newItr;
	newItr.currentT = comT;

	ComponentMetaData* meta = findMeta(comT);
	newItr.meta = meta;

	std::vector<char*>* container = findContainer(comT);
	newItr.currentContainer = container;

	std::vector<char>* containerB = findContainerBits(comT);
	newItr.currentContainerB = containerB;

	newItr.currentElement = getElementAt(comT, 0);
	newItr.currentBlock = newItr.currentElement;

	if (!newItr.checkUnClear()) // if clear
	{
		++newItr; // itr to a unclear element
		newItr.currentObjIndex = 0; // this must be 0
	}

	return newItr;
}

int ComponentMemoryManager::insertIntoContainer(ComponentMemoryManager::ComponentType comT, const char* obj)
{

	ComponentMetaData* meta = findMeta(comT);
	//std::vector<char*>* container = findContainer(comT);
	std::vector<char>* containerB = findContainerBits(comT);


	// check if container needs to be expanded
	if (meta->currentSize >= meta->currentMaxSize)
	{
		// expand
		expandContainer(comT);
	}

	// getFirstEmptyAndSetBitToTrue

	// find empty bit
	int bitindex = 0;
	for (unsigned char c : *containerB)
	{
		if (c != 0xFF)
			break;
		++bitindex;
	}

	unsigned char* bt = reinterpret_cast<unsigned char*>(&(*containerB)[bitindex]);

	unsigned char u = 0x1;
	int pos = 1;
	while ((u & *bt) & u)
	{
		u = u << 1;
		++pos;
	}

	*bt = *bt | u; // set bit to true

	++meta->currentSize; // increase size

	// find element and allocate
	int elementIndex = bitindex * 8 + pos - 1;
	char* newElement = getElementAt(comT, elementIndex);

	memcpy(newElement, obj, meta->settings.elementSize); // set memory block

	return elementIndex;
}

void ComponentMemoryManager::expandContainer(ComponentMemoryManager::ComponentType comT)
{

	// create new block
	ComponentMetaData* meta = findMeta(comT);
	ComponentTypeSettings set = meta->settings;

	// no expand
	if (meta->settings.mustBeContiguous) throw; // !


	// add new block to container
	char* newBlock = allocateBlock(set.elementSize * set.blockSize);
	std::vector<char*>* container = findContainer(comT);
	container->push_back(newBlock);

	// init new block to 0
	if (set.doInitData)
	{
		memset(newBlock, set.initTo, set.elementSize * set.blockSize);
	}

	// add container bits
	std::vector<char>* containerB = findContainerBits(comT);
	int n = (set.blockSize + meta->currentMaxSize) / 8 /*size of char*/; // n is new size
	++n;
	for (int i = (int)containerB->size() - 1; i < n; ++i) // add to current size
	{
		char newBits = 0;
		containerB->push_back(newBits);
	}

	// update stats
	meta->currentMaxSize += set.blockSize;
}

void ComponentMemoryManager::removeFromContainer(ComponentMemoryManager::ComponentType comT, int index)
{
	// set bit to false
	setBitAt(comT, index, false);

	// reset element bits
	ComponentMetaData* meta = findMeta(comT);
	if (meta->settings.doInitData)
	{
		char* elementToRemove = getElementAt(comT, index);
		memset(elementToRemove, meta->settings.initTo, meta->settings.elementSize);
	}

	// update stats
	--meta->currentSize;
}

void ComponentMemoryManager::removeFromContainer(ComponentMemoryManager::MemConIterator& itr)
{
	// call remove fn
	removeFromContainer(itr.currentT, itr.currentIndex);
	// update iterator
	--itr.currentObjIndex; // just removed one item -> total occupied items -1

}

void ComponentMemoryManager::freeAll()
{

	//std::map<ComponentType, std::vector<char*>*> componentContainers;
	//std::map<ComponentType, std::vector<char>*> componentContainerBits;

	for (auto it = componentContainers.begin(); it != componentContainers.end(); ++it)
	{
		std::vector<char*>* v = (*it).second;

		for (auto itv : *v)
		{
			void* freeptr = reinterpret_cast<void*>(itv);
			//free(reinterpret_cast<void*>(freeptr));
			std::free(freeptr);
		}

		//free(reinterpret_cast<void*>(v));

		v->clear();
		delete v;
	}

	for (auto it = componentContainerBits.begin(); it != componentContainerBits.end(); ++it)
	{
		std::vector<char>* freePtr = (*it).second;
		//free(freePtr);
		delete freePtr;
	}

	componentContainers.clear();
	componentContainerBits.clear();
}


// ComponentMemoryManager END
///////////////////////////////////////////////////////////////////////////////////////////////////

