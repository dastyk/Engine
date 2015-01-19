#ifndef ICDDLIST_H
#define ICDDLIST_H


template <typename T>
class ICircularDoubleDirectedList
{
public:
	static enum direction{FORWARD=1, BACKWARD=2};
	virtual ~ICircularDoubleDirectedList() {}
	virtual void addAtCurrent(const T& element) = 0;
	virtual T& getElementAtCurrent() const = 0;
	virtual bool remove(const T& element) = 0;
	virtual int size() const = 0;
	virtual void changeDirection() = 0;
	virtual void moveCurrent() = 0;
	virtual typename direction getCurrentDirection() const = 0;
};

#endif