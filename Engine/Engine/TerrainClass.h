#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#pragma once
#include <vector>

#include <fstream>
#include <Windows.h>

using namespace std;

#include "ModelClass.h"

#include <DirectXCollision.h>
#include "QuadTree.h"


struct TerrainVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord2;
};

class TerrainClass : public ModelClass
{
public:
	TerrainClass();
	~TerrainClass();

	bool Init(ID3D11Device*, QuadTree**);

	float getHeightAtPoint(const XMFLOAT3&)const;
	XMFLOAT3 GetVectorAtPoint(const XMFLOAT3&)const;



private:
	bool loadRAW(int width, int height, const char* filename, float heightScale, float heightOffset);
	bool loadBitmap(char* fileName, float heightScale, float heightOffset);

	bool fillVertexAndIndexData(ID3D11Device*, WCHAR* texFileName, WCHAR* name2, WCHAR* blendmap, QuadTree** ppQuadTree);

	
	float sampleHeight3x3(int i, int j);
	bool inBoundsOfHeightMap(int m, int n)const;

	void filterTerrain();

private:
	int mWidth, mHeight;
	float mHeightScale, mHeightOffset;

	float** mHeightMap;

	void fillIndices(UINT oX, UINT oY, UINT x, UINT y, unsigned long*& indices);
};

#endif