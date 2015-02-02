#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#pragma once


#include <iostream>
#include <fstream>
#include <DirectXMath.h>
#include <string>
#include <vector>

using namespace std;
using namespace DirectX;

typedef unsigned int UINT;
typedef unsigned long ULONG;


struct SmfHeader
{
	unsigned long VertexCount;
	unsigned long IndexCount;
	unsigned long bfOffBits;
	unsigned int ObjectCount;
};
struct SubsetTableDesc
{
	UINT SubsetID;
	ULONG VertexStart;
	ULONG VertexCount;
	ULONG FaceStart;
	ULONG FaceCount;
};

struct MatrialDesc
{
	XMFLOAT3 Ambient;
	XMFLOAT3 Diffuse;
	XMFLOAT3 Specular;
	float SpecPower;
	XMFLOAT3 Reflectivity;
	float AlphaClip;
	UINT DiffuseSize;
	UINT NormalSize;
};


struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
	unsigned int ID;
};

struct VertexType
{
	float x, y, z;
};

struct FaceType
{
	int vIndex1, vIndex2, vIndex3, vIndex4;
	int tIndex1, tIndex2, tIndex3, tIndex4;
	int nIndex1, nIndex2, nIndex3, nIndex4;
	int Count;
};


bool LoadModel(char* filename, int& vertexCount, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray, int& objectCount, vector<wstring> &fileName);

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, int& vertexCounts, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray);

bool LoadSmfModel(char* filename, int& vertexCount, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray, int& objectCount, vector<wstring> &fileName);


void InsertData(Vertex* pVertex, VertexType* pPoint, VertexType* pTex, VertexType* pNorm);


// String manipulation functions
std::string GetExtension(const std::string& filename);
std::string removeExtension(const std::string& filename);

#endif