#include "TerrainClass.h"


TerrainClass::TerrainClass() : ModelClass()
{
	mHeightMap = 0;
	mNrOfDetailLevels = TERRAIN_LEVEL_OF_DETAIL_COUNT;
}


TerrainClass::~TerrainClass()
{
	
	if (mHeightMap)
	{
		for (int i = 0; i < mWidth; i++)
		{
			delete[] mHeightMap[i];
		}
		delete[]mHeightMap;
		mHeightMap = 0;
	}	

	if (mHeightMapNormal)
	{
		for (int i = 0; i < mWidth; i++)
		{
			delete[] mHeightMapNormal[i];
		}
		delete[]mHeightMapNormal;
		mHeightMapNormal = 0;
	}
	
}

bool TerrainClass::Init(ID3D11Device* pDevice, QuadTree** ppQuadTree)
{

	bool result;

	result = loadRAW(513, 513, "data/resources/world11.raw", 0.5f, -128.0f);
	if (!result)
	{
		return false;
	}

	//result = loadBitmap("data/resources/world10bmp.bmp",0.3,0);
	//if (!result)
	//{
	//	return false;
	//}

	filterTerrain();
	//filterTerrain();
//	filterTerrain();
	result = fillVertexAndIndexData(pDevice, L"Moon_floor_2_jaqx_tilable_1024.png", L"seamless_mountain_rock_by_hhh316-d31i6ci.jpg",L"data/resources/blendmap1.jpg", ppQuadTree);
	if (!result)
	{
		return false;
	}


	return true;
}


bool TerrainClass::loadRAW(int width, int height, const char* filename, float heightScale, float heightOffset)
{
	mWidth = width;
	mHeight = height;
	mHeightScale = heightScale;
	mHeightOffset = heightOffset;

	vector<unsigned char> vertexHeights(mWidth*mHeight);

	ifstream fin;
	fin.open(filename, ios_base::binary);
	if (!fin)
	{
		MessageBox(0, L"Failed to open file.", (WCHAR*)filename, 0);
		return false;
	}

	fin.read((char*)&vertexHeights[0], (streamsize)vertexHeights.size());
	fin.close();

	mHeightMap = new float*[mHeight];
	for (int i = 0; i < mHeight; i++)
		mHeightMap[i] = new float[mWidth];

	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			int index = j*mWidth + i;
			float height = vertexHeights[index];
			mHeightMap[i][j] = (float)(((vertexHeights[index]) + mHeightOffset) * mHeightScale);
		}
	}

	vertexHeights.clear();

	return true;
}

bool TerrainClass::loadBitmap(char* fileName, float heightScale, float heightOffset)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k;
	unsigned char* bitmapImage;

	mHeightScale = heightScale;
	mHeightOffset = heightOffset;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	mWidth = bitmapInfoHeader.biWidth;
	mHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = mWidth * mHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	mHeightMap = new float*[mWidth];
	for (int i = 0; i < mWidth; i++)
		mHeightMap[i] = new float[mHeight];

	if (!mHeightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (i = 0; i<mWidth; i++)
	{
		for (j = 0; j<mHeight; j++)
		{
			float height = bitmapImage[k];

			mHeightMap[i][j] = (float)(height * mHeightScale + mHeightOffset);

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void TerrainClass::filterTerrain()
{
	float** FilteredHeightMap = new float*[mHeight];
	for (int i = 0; i < mHeight; i++)
		FilteredHeightMap[i] = new float[mWidth];

	for (int j = 0; j < mHeight; j++)
	{
		for (int i = 0; i < mWidth; i++)
		{
			FilteredHeightMap[i][j] = sampleHeight3x3(i, j);
		}
	}

	for (int i = 0; i < mWidth; i++)
	{
		delete[] mHeightMap[i];
	}
	if (mHeightMap)
	{
		delete[]mHeightMap;
		mHeightMap = 0;
	}
	mHeightMap = FilteredHeightMap;
}

float TerrainClass::sampleHeight3x3(int i, int j)
{
	float average = 0.0f;
	float sample = 0.0f;

	for (int m = i-1; m <= i+1; m++)
	{
		for (int n = j-1; n <= j+1; n++)
		{
			if (inBoundsOfHeightMap(m, n))
			{
				average += mHeightMap[m][n];
				sample += 1.0f;
			}
		}
	}
	
	return average / sample;
}

bool TerrainClass::inBoundsOfHeightMap(int m, int n)const
{
	if (m < 0 || n < 0)
		return false;
	if (m >= mWidth || n >= mHeight)
		return false;

	return true;
}

bool TerrainClass::fillVertexAndIndexData(ID3D11Device* pDevice, WCHAR* texFileName, WCHAR* name2, WCHAR* blendmap, QuadTree** ppQuadTree)
{
	bool result;
	TerrainVertex* vertices = 0;
	mStride = sizeof(TerrainVertex);
	mVertexCount = mWidth*mHeight;

	
	vertices = new TerrainVertex[mVertexCount];

	float dx = 2.0f;
	float dz = 2.0f;

	float width = (mWidth - 1) / 2.0f;
	float depth = (mHeight - 1) / 2.0f;

	for (int j = 0; j < mHeight; j++)
	{
		for (int i = 0; i < mWidth; i++)
		{
			int index = j*mWidth + i;
			vertices[index].Pos = XMFLOAT3((float)i, (float)mHeightMap[i][j], (float)j);
			vertices[index].texCoord = XMFLOAT2(i / width  * dx, j / depth * dz);
			vertices[index].TexCoord2 = XMFLOAT2(i/25.0f, mHeight - j/25.0f);
			vertices[index].Normal = XMFLOAT3(0, 0, 0);
			vertices[index].tangent = XMFLOAT3(0, 0, 0);
			vertices[index].binormal = XMFLOAT3(0, 0, 0);
		}
	}

	//if (dim / ((2 ^ (detail - 1)) ^ 2) >= detail then detail level is accepted)
	bool done = false;
	mNrOfDetailLevels = 0;
	while (!done)
	{
		float w = (float)mWidth - 1;
		float pow1 = powf(2.0f, (float)mNrOfDetailLevels);
		float pow2 = powf(pow1, 2.0f);
		float t = w / pow2;
		if (t < mNrOfDetailLevels + 1)
		{
			done = true;
		}
		else
		{
			mNrOfDetailLevels++;
		}
	}

	mNrOfDetailLevels = TERRAIN_LEVEL_OF_DETAIL_COUNT;
	int count = (mWidth-1)*(mHeight-1);

	mIndexCount = new UINT[mNrOfDetailLevels];
	mIndexBuffer = new ID3D11Buffer*[mNrOfDetailLevels];

	for (UINT i = 0; i < mNrOfDetailLevels; i++)
	{
		UINT detail = (UINT)pow((float)2, (float)i);

		mIndexCount[i] = (count * 6) / (detail*detail);

		

		unsigned long* indices = new unsigned long[mIndexCount[i]];
		fillIndices(0, 0, mWidth - 1, mHeight - 1, indices, detail);

		if (i == 0)
		{
			CalcNormalTangentBinormal(vertices, indices, i);
			 mHeightMapNormal = new XMFLOAT3*[mWidth];
			for (int k = 0; k < mWidth; k++)
				mHeightMapNormal[k] = new XMFLOAT3[mHeight];

			for (int k = 0; k < mHeight; k++)
			{
				for (int l = 0; l < mWidth; l++)
				{
					UINT d = k*mWidth + l;
					mHeightMapNormal[l][k] = vertices[d].Normal;
				}
			}
		}
		

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		iinitData.SysMemPitch = 0;
		iinitData.SysMemSlicePitch = 0;

		result = createIndexBuffer(pDevice, &iinitData, &mIndexBuffer[i], sizeof(unsigned long)*mIndexCount[i]);
		if (!result)
		{
			return false;
		}

		delete[]indices;

		
	}


	(*ppQuadTree) = new QuadTree;
	(*ppQuadTree)->Init(mVertexCount, (XMFLOAT3*)vertices, sizeof(TerrainVertex), mIndexCount, mNrOfDetailLevels);


	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;

	result = createVertexBuffer(pDevice, &vinitData, &mVertexBuffer, mStride * mVertexCount);
	if (!result)
	{
		return false;
	}
	



	delete[]vertices;


	mTexture = new TextureClass();
	if (!mTexture)
	{
		return false;
	}


	vector<wstring> tex;
	tex.push_back(name2);
	tex.push_back(texFileName);
	
	result = mTexture->Init(pDevice, tex, blendmap);
	if (!result)
	{
		return false;
	}


	mDetailMap = new TextureClass();
	if (!mDetailMap)
	{
		return false;
	}


	vector<wstring> tex2;
	tex2.push_back(L"stone_texture___seamless_by_agf81-d39vfot.jpg");

	result = mDetailMap->Init(pDevice, tex2, NULL);
	if (!result)
	{
		return false;
	}


	mNormalMap = new TextureClass();
	if (!mNormalMap)
	{
		return false;
	}


	vector<wstring> tex3;
	tex3.push_back(L"TerrainNormalMap.jpg");

	result = mNormalMap->Init(pDevice, tex3, NULL);
	if (!result)
	{
		return false;
	}
	return true;
}

void TerrainClass::CalcNormalTangentBinormal(TerrainVertex* vertices, unsigned long* indices, UINT i)
{
	UINT index = 0;


	while (index < mIndexCount[i])
	{
		XMVECTOR pos1 = XMLoadFloat3(&(vertices[indices[index]].Pos));
		XMVECTOR pos2 = XMLoadFloat3(&(vertices[indices[index + 1]].Pos));
		XMVECTOR pos3 = XMLoadFloat3(&(vertices[indices[index + 2]].Pos));

		XMVECTOR vec1 = pos2 - pos1;
		XMVECTOR vec2 = pos3 - pos1;

		XMVECTOR norm = XMVector3Normalize(XMVector3Cross(vec1, vec2));

		for (int k = 0; k < 3; k++)
		{
			XMVECTOR origNorm = XMLoadFloat3(&(vertices[indices[index + k]].Normal));
			XMVECTOR finNorm = XMVector3Normalize((norm + origNorm));

			XMStoreFloat3(&(vertices[indices[index + k]].Normal), norm);
		}
		XMVECTOR pos4 = XMLoadFloat3(&(vertices[indices[index + 5]].Pos));

		vec1 = pos4 - pos3;
		vec2 = pos2 - pos3;

		norm = XMVector3Normalize(XMVector3Cross(vec1, vec2));

		for (int k = 3; k < 6; k++)
		{
			XMVECTOR origNorm = XMLoadFloat3(&(vertices[indices[index + k]].Normal));
			XMVECTOR finNorm = XMVector3Normalize((norm + origNorm));

			XMStoreFloat3(&(vertices[indices[index + k]].Normal), norm);
		}






		index += 6;
	}

	UINT faceCount = mIndexCount[i] / 3;
	index = 0;

	for (UINT i = 0; i < faceCount; i++)
	{
		Point p1, p2, p3;
		p1.pos = XMLoadFloat3(&(vertices[indices[index]].Pos));
		p1.tex = XMLoadFloat2(&(vertices[indices[index]].texCoord));

		p2.pos = XMLoadFloat3(&(vertices[indices[index + 1]].Pos));
		p2.tex = XMLoadFloat2(&(vertices[indices[index + 1]].texCoord));

		p3.pos = XMLoadFloat3(&(vertices[indices[index + 2]].Pos));
		p3.tex = XMLoadFloat2(&(vertices[indices[index + 2]].texCoord));

		XMFLOAT3 tangent, binormal;

		calcTangetBinormal(&p1, &p2, &p3, tangent, binormal);

		vertices[indices[index]].tangent = tangent;
		vertices[indices[index]].binormal = binormal;
		index++;
		vertices[indices[index]].tangent = tangent;
		vertices[indices[index]].binormal = binormal;
		index++;
		vertices[indices[index]].tangent = tangent;
		vertices[indices[index]].binormal = binormal;
		index++;
	}
}

void TerrainClass::calcTangetBinormal(Point* p1, Point* p2, Point* p3, XMFLOAT3& tangent, XMFLOAT3& binormal)
{
	XMVECTOR vec1 = p2->pos - p1->pos;
	XMVECTOR vec2 = p3->pos - p1->pos;

	XMVECTOR t1 = p2->tex - p1->tex;
	XMVECTOR t2 = p3->tex - p1->tex;
	
	float cp = XMVectorGetY(t1)*XMVectorGetX(t2) - XMVectorGetX(t1)*XMVectorGetY(t2);

	if (cp != 0.0f) {
		float mul = 1.0f / cp;
		
		XMVECTOR tan = (vec1 * (-XMVectorGetY(t2)) + vec2 * XMVectorGetY(t1))*mul;
		XMVECTOR bi = (vec1 * (-XMVectorGetX(t2)) + vec2 * XMVectorGetX(t1))*mul;
		tan = XMVector3Normalize(tan);
		bi = XMVector3Normalize(bi);

		XMStoreFloat3(&tangent, tan);
		XMStoreFloat3(&binormal, bi);
	}

}


float TerrainClass::getHeightAtPoint(const XMFLOAT3& pos)const
{
	int x = (int)pos.x;
	int z = (int)pos.z;

	if (!inBoundsOfHeightMap(x, z))
			return 0.0;
	if (!inBoundsOfHeightMap(x + 1, z + 1))
		return 0.0;

	float fTriY0 = (mHeightMap[x][z]);
	float fTriY1 = (mHeightMap[x + 1][z]);
	float fTriY2 = (mHeightMap[x][z + 1]);
	float fTriY3 = (mHeightMap[x + 1][z + 1]);

	float fHeight;
	float fSqX = pos.x - truncf(pos.x);
	float fSqZ = pos.z - truncf(pos.z);
	if ((fSqX + fSqZ) < 1)
	{
		fHeight = fTriY0;
		fHeight += (fTriY1 - fTriY0) * fSqX;
		fHeight += (fTriY2 - fTriY0) * fSqZ;
	}
	else
	{
		fHeight = fTriY3;
		fHeight += (fTriY1 - fTriY3) * (1.0f - fSqZ);
		fHeight += (fTriY2 - fTriY3) * (1.0f - fSqX);
	}
	return fHeight;
}

bool TerrainClass::GetVectorAtPoint(const XMFLOAT3& forward, const XMFLOAT3& pos, XMFLOAT3& out)const
{
	int x = (int)pos.x;
	int z = (int)pos.z;

	if (!inBoundsOfHeightMap(x, z))
		return false;
	if (!inBoundsOfHeightMap(x + 1, z + 1))
		return false;

	XMFLOAT3 fTriY0 = (mHeightMapNormal[x][z]);
	XMFLOAT3 fTriY1 = (mHeightMapNormal[x + 1][z]);
	XMFLOAT3 fTriY2 = (mHeightMapNormal[x][z + 1]);
	XMFLOAT3 fTriY3 = (mHeightMapNormal[x + 1][z + 1]);

	float fSqX = pos.x - truncf(pos.x);
	float fSqZ = pos.z - truncf(pos.z);
	if ((fSqX + fSqZ) < 1)
	{
		XMVECTOR v1 = XMLoadFloat3(&fTriY0);
		XMVECTOR v2 = XMLoadFloat3(&fTriY1);
		XMVECTOR v3 = XMLoadFloat3(&fTriY2);

		XMVECTOR norm = XMVector3Normalize(v1 + v2 + v3);

		XMVECTOR f = XMLoadFloat3(&forward);
		XMVECTOR tan = XMVector3Cross(f, norm);
		XMVECTOR fn = XMVector3Normalize( XMVector3Cross(norm, tan));
		XMStoreFloat3(&out, fn);
	}
	else
	{
		XMVECTOR v1 = XMLoadFloat3(&fTriY3);
		XMVECTOR v2 = XMLoadFloat3(&fTriY1);
		XMVECTOR v3 = XMLoadFloat3(&fTriY2);

		XMVECTOR norm = XMVector3Normalize(v1 + v2 + v3);

		XMVECTOR f = XMLoadFloat3(&forward);
		XMVECTOR tan = XMVector3Cross(f, norm);
		XMVECTOR fn = XMVector3Cross(tan, norm);
		XMStoreFloat3(&out, fn);
	}

	return true;
}

void TerrainClass::fillIndices(UINT oX, UINT oY, UINT x, UINT y, unsigned long*& indices, UINT LevelOfQuality)
{
	if (x > LevelOfQuality || y > LevelOfQuality)
	{
		UINT count = (x*y * 6) / (LevelOfQuality*LevelOfQuality);
		UINT c2 = ((x / 2)*(y / 2) * 6) / (LevelOfQuality*LevelOfQuality);
		UINT index = 0;
		for (UINT j = 0; j < 2; j++)
		{
			for (UINT i = 0; i < 2; i++)
			{
				unsigned long* Indices = new unsigned long[c2];
				UINT oW = oX + i*(x / 2);
				UINT oH = oY + j*(mWidth)*(y/2);
				fillIndices(oW, oH, x / 2, y / 2, Indices,LevelOfQuality);
				for (UINT k = 0; k < c2; k++)
				{
					indices[index] = Indices[k];
					index++;
				}
				delete[]Indices;
			}
		}
	}
	else
	{
		int d = oY + oX;
		indices[0] = (unsigned long)d;
		indices[1] = indices[4] = (unsigned long)mWidth*LevelOfQuality + d;
		indices[2] = indices[3] = (unsigned long)d + LevelOfQuality;
		indices[5] = (unsigned long)mWidth*LevelOfQuality + d + LevelOfQuality;
	}
}
