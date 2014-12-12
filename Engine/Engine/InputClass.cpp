#include "InputClass.h"


InputClass::InputClass(int screenWidth, int screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mMousePos.x = 0;
	mMousePos.y = 0;

	for (int i = 0; i < 256; i++)
		mKeys[i] = false;


}

InputClass::~InputClass()
{
}

void InputClass::keyDown(unsigned int key)
{
	mKeys[key] = true;
}

void InputClass::keyUp(unsigned int key)
{
	mKeys[key] = false;
}

bool InputClass::isKeyDown(unsigned int key)
{
	return mKeys[key];
}

POINT InputClass::getMousePos()const
{
	return mMousePos;
}

void InputClass::setMousePos(POINT& pos)
{
	mMousePos = pos;
}

void InputClass::getScreenDim(int& width, int& height)
{
	width = mScreenWidth;
	height = mScreenHeight;
} 

void InputClass::setScreenDim(int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
}
