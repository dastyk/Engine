#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#pragma once

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_C 0x43
#define VK_F 0x46
#define VK_L 0x4C

#include <Windows.h>

class InputClass
{
public:
	InputClass(int screenWidth, int screenHeight);
	~InputClass();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool isKeyDown(unsigned int);

	POINT getMousePos()const;
	void setMousePos(POINT&);

	void getScreenDim(int&, int&);
	void setScreenDim(int, int);

private:
	int mScreenWidth,mScreenHeight;
	POINT mMousePos;

	bool mKeys[256];
};

#endif