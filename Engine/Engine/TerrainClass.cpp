#include "TerrainClass.h"


TerrainClass::TerrainClass() : ModelClass()
{
	mHeightMap = 0;
}


TerrainClass::~TerrainClass()
{
	for (int i = 0; i < mWidth; i++)
	{
		delete[] mHeightMap[i];
	}
	if (mHeightMap)
	{
		delete[]mHeightMap;
		mHeightMap = 0;
	}	
}

bool TerrainClass::Init(ID3D11Device* pDevice, QuadTree** ppQuadTree)
{

	bool result;

	result = loadRAW(257, 257, "data/resources/world10raw.raw", 0.5f, 0);
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
	filterTerrain();
	filterTerrain();
	result = fillVertexAndIndexData(pDevice, L"seamless_mountain_rock_by_hhh316-d31i6ci.jpg", L"seemless_4.jpg",L"data/resources/blendmap1.jpg", ppQuadTree);
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
			mHeightMap[i][j] = (float)(((vertexHeights[index])) * mHeightScale + mHeightOffset);
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
	XMFLOAT3* pPoints = new XMFLOAT3[mVertexCount];

	float dx = 4.0f;
	float dz = 4.0f;

	float width = (mWidth - 1) / 2.0f;
	float depth = (mHeight - 1) / 2.0f;

	for (int j = 0; j < mHeight; j++)
	{
		for (int i = 0; i < mWidth; i++)
		{
			int index = j*mWidth + i;
			//mHeightMap[i][j] = i;
			vertices[index].Pos = XMFLOAT3((float)i, (float)mHeightMap[i][j], (float)j);
			pPoints[index] = vertices[index].Pos;
			vertices[index].texCoord = XMFLOAT2(i / width  * dx, j / depth * dz);
			vertices[index].TexCoord2 = XMFLOAT2(i / (float)mWidth, j/(float)mHeight);
			vertices[index].Normal = XMFLOAT3(0, 0, 0);
		}
	}

	int count = (mWidth-1)*(mHeight-1);
	mIndexCount = count * 6;

	(*ppQuadTree) = new QuadTree;
	(*ppQuadTree)->Init(mVertexCount, (XMFLOAT3*)vertices, sizeof(TerrainVertex), mIndexCount);

	unsigned long* indices = new unsigned long[mIndexCount];

	int w2 = (mWidth - 1) / 2;
	int w3 = w2 / 2;

	int h2 = (mHeight - 1) / 2;
	int h3 = h2 / 2;

	fillIndices(0, 0, mWidth - 1, mHeight - 1, indices);


	int index = 0;

	for (int j = 0; j < (mHeight - 1); j++)
	{
		for (int i = 0; i < (mWidth - 1); i++)
		{
			XMVECTOR pos1 = XMLoadFloat3(&(vertices[indices[index]].Pos));
			XMVECTOR pos2 = XMLoadFloat3(&(vertices[indices[index+1]].Pos));
			XMVECTOR pos3 = XMLoadFloat3(&(vertices[indices[index+2]].Pos));

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
	}


	
	delete[]pPoints;


	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;

	result = createVertexBuffer(pDevice, &vinitData, mStride * mVertexCount);
	if (!result)
	{
		return false;
	}

	delete[]vertices;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	iinitData.SysMemPitch = 0;
	iinitData.SysMemSlicePitch = 0;

	result = createIndexBuffer(pDevice, &iinitData, sizeof(unsigned long)*mIndexCount);
	if (!result)
	{
		return false;
	}

	delete[]indices;




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

	return true;
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

void TerrainClass::fillIndices(UINT oX, UINT oY, UINT x, UINT y, unsigned long*& indices)
{
	if (x > 1 || y > 1)
	{
		UINT count = x*y * 6;
		UINT c2 = (x / 2)*(y / 2) * 6;
		UINT index = 0;
		for (UINT j = 0; j < 2; j++)
		{
			for (UINT i = 0; i < 2; i++)
			{
				unsigned long* Indices = new unsigned long[c2];
				UINT oW = oX + i*(x / 2);
				UINT oH = oY + j*(mWidth)*(y/2);
				fillIndices(oW, oH, x / 2, y / 2, Indices);
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
		indices[1] = indices[4] = (unsigned long)mWidth + d;
		indices[2] = indices[3] = (unsigned long)d + 1;
		indices[5] = (unsigned long)mWidth + d + 1;
	}
}