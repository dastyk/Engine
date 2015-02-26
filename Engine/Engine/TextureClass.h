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

	bool Init(ID3D11Device* pDevice, vector<wstring> fileName, WCHAR* blendMapName);

	ID3D11ShaderResourceView**	GetShaderResourceView()const;
	int GetTextureCount()const;

	ID3D11ShaderResourceView*	GetBlendMapShaderResourceView()const;
	bool blendEnabled()const;

private:
	int mTextureCount;
	bool mUseBlendMap;
	std::wstring GetExtension(const std::wstring& filename);
	std::wstring removeExtension(const std::wstring& filename);
private:
	ID3D11ShaderResourceView** mSRV;
	ID3D11Texture2D** mTexture;
	ID3D11ShaderResourceView* mBlendMapSRV;

};

#endif