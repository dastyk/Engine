#ifndef _MEMORYHANDLER_H_
#define _MEMORYHANDLER_H_
#pragma once

class MemoryHandler
{
public:
	MemoryHandler(const size_t sizeOfElement,const UINT Count)
	{
		mCount = Count;
		mSizeOfElement = sizeOfElement;

		memBlock = malloc(sizeOfElement*mCount);
		firstFree = 0;

		memIndex = new UINT[mCount];
		for (UINT i = 0; i < mCount; i++)
		{
			memIndex[i] = i + 1;
		}
	}

	~MemoryHandler()
	{
		delete[] memBlock;
		delete[] memIndex;
	}

	void* memBlock;
	UINT* memIndex;
	UINT firstFree;

	UINT mCount;
	size_t mSizeOfElement;

	void* GetFirstEmptyMemoryBlock(){
		void* temp = (void*)((UINT)memBlock + mSizeOfElement*firstFree);
		firstFree = memIndex[firstFree];
		return temp;
	}

	void* GetMemoryBlock(UINT i){
		void* temp = (void*)((UINT)memBlock + mSizeOfElement*i);
		return temp;
	}
	//void FreeMemory(){}
private:

};

#endif