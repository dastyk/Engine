#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "WICTextureLoader.h"
#include <string>
#include <vector>

using namespace std;

class TextureClass
{
public:
	TextureClass();
	~TextureClass();

	bool Init(ID3D11Device* pDevice, vector<wstring> fileName);

	ID3D11ShaderResourceView**	GetShaderResourceView()const;
	int GetTextureCount()const;

private:
	int mTextureCount;

private:
	ID3D11ShaderResourceView** mSRV;
};

#endif