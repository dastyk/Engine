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

#define TERRAIN_LEVEL_OF_DETAIL_HIGH 1
#define TERRAIN_LEVEL_OF_DETAIL_MEDIUM 2
#define TERRAIN_LEVEL_OF_DETAIL_LOW 4
#define TERRAIN_LEVEL_OF_DETAIL_VERY_LOW 8
#define TERRAIN_LEVEL_OF_DETAIL_COUNT 6  // dim/((2^detail-1)^2) // t.ex. om mappen är 256x256 loppar man fram max antal levels genom att börja på 1 if 256/(2^1-1)^2 >= 1 then ok, sen 256/(2^2-1)^2 >= 2

// Generellt if(dim/((2^(detail-1))^2) >= detail then detail level is accepted)


struct TerrainVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord2;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};


struct Point
{
	XMVECTOR pos;
	XMVECTOR tex;
};
class TerrainClass : public ModelClass
{
public:
	TerrainClass();
	~TerrainClass();

	bool Init(ID3D11Device*, QuadTree**);

	float getHeightAtPoint(const XMFLOAT3&)const;
	bool GetVectorAtPoint(const XMFLOAT3& forward, const XMFLOAT3& pos, XMFLOAT3& out)const;



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
	XMFLOAT3** mHeightMapNormal;

	void fillIndices(UINT oX, UINT oY, UINT x, UINT y, unsigned long*& indices, UINT LevelOfQuality);
	void CalcNormalTangentBinormal(TerrainVertex* vertices, unsigned long* indices, UINT i);


	void calcTangetBinormal(Point*, Point*, Point*, XMFLOAT3&, XMFLOAT3&);
};

#endif