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

struct BoneRead
{
	XMFLOAT4X4 localOffset;
	int ParentBone;
};

struct Bone
{
	XMFLOAT4X4 localOffset;
	int ParentBone;
	XMFLOAT4X4 globalOffset;
	XMFLOAT4X4 invBindPose;
};

struct Frame
{
	float time;
	XMFLOAT3 Pos;
	XMFLOAT3 Scale;
	XMFLOAT4 Quat;
};

struct BoneFrame
{
	UINT frameCount;
	Frame* Frames;
};

struct AnimClipRead
{
	BoneFrame* bones;
};


struct SmfHeader
{
	unsigned long VertexCount;
	unsigned long IndexCount;
	unsigned long bfOffBits;
	unsigned int ObjectCount;
	UINT BoneCount;
	UINT AnimationClips;
};

struct MatrialDesc
{
	XMFLOAT3 Ambient;
	XMFLOAT3 Diffuse;
	XMFLOAT3 Specular;
	float SpecPower;
	XMFLOAT3 Reflectivity;
	float AlphaClip;
};

struct MatrialDescPadded
{
	XMFLOAT3 Ambient;
	float pad1;
	XMFLOAT3 Diffuse;
	float pad2;
	XMFLOAT3 Specular;
	float pad3;

	XMFLOAT3 Reflectivity;
	float pad4;
	
	XMFLOAT4 SpecPower_AlphaClip_Unused_Unused;
};
struct UINT4
{
	UINT x, y, z, w;
};

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
	XMFLOAT4 BlendWeights;
	UINT4 BlendIndices;
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


bool LoadModel(char* filename, UINT& vertexCount, Vertex** ppVertexArray, UINT& indexCount, unsigned long** ppIndexArray, UINT& objectCount, vector<wstring> &fileName, MatrialDesc** ppMaterials, BoneRead** ppBones, UINT& boneCount, AnimClipRead** ppAnimClip, UINT& animClips);

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, int& vertexCounts, Vertex** ppVertexArray, int& indexCount, unsigned long** ppIndexArray);

bool LoadSmfModel(char* filename, UINT& vertexCount, Vertex** ppVertexArray, UINT& indexCount, unsigned long** ppIndexArray, UINT& objectCount, vector<wstring> &fileName, MatrialDesc** ppMaterials, BoneRead** ppBones, UINT& boneCount, AnimClipRead** ppAnimClip, UINT& animClips);


void InsertData(Vertex* pVertex, VertexType* pPoint, VertexType* pTex, VertexType* pNorm);


// String manipulation functions
std::string GetExtension(const std::string& filename);
std::string removeExtension(const std::string& filename);

#endif