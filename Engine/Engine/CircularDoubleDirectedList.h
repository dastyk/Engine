#ifndef _CIRCULARDOUBLEDIRECTEDLIST_H_
#define _CIRCULARDOUBLEDIRECTEDLIST_H_

#include "ICircularDoubleDirectedList.h"

template<typename T>
class CircularDoubleDirectedList : public ICircularDoubleDirectedList<T>
{
private:
	class Node
	{
	public:
		T mItem;
		Node* mRight;
		Node* mLeft;
		Node(T pItem){mItem = pItem; mRight = nullptr; mLeft = nullptr; }
		~Node(){}
	};
public:
	CircularDoubleDirectedList();
	CircularDoubleDirectedList(const CircularDoubleDirectedList& other);
	CircularDoubleDirectedList<T>& operator=(const CircularDoubleDirectedList& other);
	~CircularDoubleDirectedList();
	void addAtCurrent(const T& element);
	T& getElementAtCurrent() const;
	bool remove(const T& element);
	int size() const;
	void changeDirection();
	void moveCurrent();
	typename direction getCurrentDirection() const {return currentDirection;}

private:
	Node* current;
	int nrOfElements;
	typename direction currentDirection;

	// Helper func
	void deleteNodes(Node* p);
};

template<typename T>
CircularDoubleDirectedList<T>::CircularDoubleDirectedList()
{
	current = nullptr;
	nrOfElements = 0;
	currentDirection = FORWARD;
}


template<typename T>
CircularDoubleDirectedList<T>::CircularDoubleDirectedList(const CircularDoubleDirectedList& other)
{
	this->nrOfElements = other.nrOfElements;
	this->currentDirection = other.currentDirection;

	if(other.nrOfElements != 0)
	{
		Node* walker1 = other.current;
		this->current = new Node(walker1->mItem);
		Node* walker2 = this->current;
		for(int i = 1; i < nrOfElements; i++)
		{
			walker1 = walker1->mRight;
			walker2->mRight = new Node(walker1->mItem);
			walker2->mRight->mLeft = walker2;
			walker2 = walker2->mRight;
		}
		walker2->mRight = this->current;
		this->current->mLeft = walker2;
	}
}

template<typename T>
CircularDoubleDirectedList<T>& CircularDoubleDirectedList<T>::operator=(const CircularDoubleDirectedList& other)
{
	if(this == &other)
		return *this;
	
	if(this->nrOfElements != 0)
	{
		deleteNodes(this->current);
	}

	this->nrOfElements = other.nrOfElements;
	this->currentDirection = other.currentDirection;
	
	if(other.nrOfElements != 0)
	{
		Node* walker1 = other.current;
		this->current = new Node(walker1->mItem);
		Node* walker2 = this->current;
		for(int i = 1; i < nrOfElements; i++)
		{
			walker1 = walker1->mRight;
			walker2->mRight = new Node(walker1->mItem);
			walker2->mRight->mLeft = walker2;
			walker2 = walker2->mRight;
		}
		walker2->mRight = this->current;
		this->current->mLeft = walker2;
	}

	return *this;
}

template<typename T>
void CircularDoubleDirectedList<T>::deleteNodes(Node* p)
{
	if(p->mRight == current)
	{
		delete p;
		p = nullptr;
	}
	else
	{
		deleteNodes(p->mRight);
		delete p;
		p = nullptr;
	}
}

template<typename T>
CircularDoubleDirectedList<T>::~CircularDoubleDirectedList()
{
	if(nrOfElements != 0)
		deleteNodes(current);
}

template<typename T>
void CircularDoubleDirectedList<T>::addAtCurrent(const T& element)
{
	Node* temp = new Node(element);
	if(nrOfElements == 0)
	{
		current = temp;
		current->mLeft = current;
		current->mRight = current;
	}
	else
	{
		Node* last = current->mLeft;
		temp->mRight = current;
		temp->mLeft = current->mLeft;
		last->mRight = temp;
		current->mLeft = temp;
		current = temp;
	}
	nrOfElements++;
}

template<typename T>
T& CircularDoubleDirectedList<T>::getElementAtCurrent() const
{
	if(nrOfElements==0)
		throw "Exception: empty list";
	return current->mItem;
}


template<typename T>
bool CircularDoubleDirectedList<T>::remove(const T& element)
{
	if(nrOfElements==0)
		throw "Exception: empty list";


	Node* walker = current;
	for(int i = 0; i < nrOfElements; i++)
	{
		if(walker->mItem == element)
		{
			Node* prev = walker->mLeft;
			Node* next = walker->mRight;
			prev->mRight = next;
			next->mLeft = prev;
			if(walker == current)
			{
				current = current->mRight;
			}
			delete walker;
			nrOfElements--;
			return true;
		}
		walker = walker->mRight;
	}
	return false;
}

template<typename T>
int CircularDoubleDirectedList<T>::size() const
{
	return nrOfElements;
}

template<typename T>
void CircularDoubleDirectedList<T>::changeDirection()
{
	currentDirection = (currentDirection == FORWARD)? BACKWARD : FORWARD;
}

template<typename T>
void CircularDoubleDirectedList<T>::moveCurrent()
{
	if(currentDirection == FORWARD)
	{
		current = current->mRight;
	}
	else
	{
		current = current->mLeft;
	}
}

#endif