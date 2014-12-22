#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#pragma once
#include <vector>

#include <fstream>
#include <Windows.h>

using namespace std;

#include "ModelClass.h"

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

	bool Init(ID3D11Device*);

	float getHeightAtPoint(const XMFLOAT3&)const;
	XMFLOAT3 GetVectorAtPoint(const XMFLOAT3&)const;

	void SetAsModelToBeDrawn(ID3D11DeviceContext*, int, unsigned long*);

	bool SetAsModelToBeDrawnFromViewFrustum(ID3D11DeviceContext*, BoundingFrustum& frustum);

	int GetIndexCount()const;

private:
	bool loadRAW(int width, int height, const char* filename, float heightScale, float heightOffset);
	bool loadBitmap(char* fileName, float heightScale, float heightOffset);

	bool fillVertexAndIndexData(ID3D11Device*, WCHAR* texFileName, WCHAR* name2, WCHAR* blendmap);

	
	float sampleHeight3x3(int i, int j);
	bool inBoundsOfHeightMap(int m, int n)const;

	void filterTerrain();

private:
	int mWidth, mHeight;
	float mHeightScale, mHeightOffset;
	
	int mDynIndexCount;

	float** mHeightMap;

	ID3D11Buffer* mDynIndexBuffer;

	QuadTree* mQuadTree;
};

#endif