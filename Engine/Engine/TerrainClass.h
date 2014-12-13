#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

#pragma once
#include <vector>

#include <fstream>
#include <Windows.h>

using namespace std;

#include "ModelClass.h"

class TerrainClass : public ModelClass
{
public:
	TerrainClass();
	~TerrainClass();

	bool Init(ID3D11Device*);

private:
	bool loadRAW(int width, int height, const char* filename, float heightScale, float heightOffset);
	bool loadBitmap(char* fileName);

	bool fillVertexAndIndexData(ID3D11Device*, WCHAR* texFileName, WCHAR* name2, WCHAR* blendmap);

	float sampleHeight3x3(int i, int j);
	bool inBoundsOfHeightMap(int m, int n);

	void filterTerrain();

private:
	int mWidth, mHeight;
	float mHeightScale, mHeightOffset;

	float* mHeightMap;
};

#endif