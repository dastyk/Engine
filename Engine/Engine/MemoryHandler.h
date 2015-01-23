#ifndef _MEMORYHANDLER_H_
#define _MEMORYHANDLER_H_
#pragma once

class MemoryHandler
{
public:
	MemoryHandler(size_t sizeOfElement, UINT Count)
	{
		currentCell = new MemoryCell;
		MemoryCell* temp = currentCell;
		for (UINT i = 0; i < Count; i++)
		{
			temp->AllocatedMemory = malloc(sizeOfElement);
			temp->nextCell = new MemoryCell;
			temp = temp->nextCell;
		}
	}
	~MemoryHandler()
	{
		deleteCells(currentCell);
	}

	struct MemoryCell
	{
		void*AllocatedMemory;
		MemoryCell* nextCell;
	};

	MemoryCell* currentCell;
	UINT mCount;

private:

	void deleteCells(MemoryCell* cell)
	{
		if (!cell->nextCell)
		{
			delete cell;
			cell = 0;
		}
		else
		{
			deleteCells(cell);
			delete cell;
			cell = 0;
		}
	}
};

#endif