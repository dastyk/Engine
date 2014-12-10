#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
};

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	virtual ~ModelClass();

	

	void SetAsModelToBeDrawn(ID3D11DeviceContext*);

	int GetIndexCount()const;
	TextureClass* GetTexture()const;


	bool createModel(ID3D11Device*, WCHAR* texFileName);

protected:
	bool createVertexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, UINT);
	bool createIndexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, UINT);

protected:
	TextureClass *mTexture;
	ID3D11Buffer* mVertexBuffer, *mIndexBuffer;
	int mVertexCount, mIndexCount;
	unsigned int mStride;
};

#endif