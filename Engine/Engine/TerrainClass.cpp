#include "TerrainClass.h"


TerrainClass::TerrainClass() : ModelClass()
{
}


TerrainClass::~TerrainClass()
{
	if (mHeightMap)
	{
		delete[]mHeightMap;
		mHeightMap = 0;
	}

}

bool TerrainClass::Init(ID3D11Device* pDevice)
{

	bool result;

	result = loadBitmap("data/resources/workin10.bmp");
	if (!result)
	{
		return false;
	}

	filterTerrain();

	result = fillVertexAndIndexData(pDevice, L"data/resources/flint_like_rock_4787.JPG", L"data/resources/seamless_mountain_rock_by_hhh316-d31i6ci.jpg",L"data/resources/blendmap.jpg");
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

	mHeightMap = new float[mWidth*mHeight];

	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			int index = j*mWidth + i;
			mHeightMap[index] = (float)((vertexHeights[index] - 128.0f));// *mHeightScale + mHeightOffset;
			int hej = 123;
		}
	}

	vertexHeights.clear();

	return true;
}

bool TerrainClass::loadBitmap(char* fileName)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

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
	mHeightMap = new float[mWidth * mHeight];
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
			height = bitmapImage[k];

			index = (mWidth * j) + i;

			mHeightMap[index] = (float)(height-128.0f)/3.0f;

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
	float* FilteredHeightMap = new float[mWidth*mHeight];

	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			int index = j*mWidth + i;
			FilteredHeightMap[index] = sampleHeight3x3(i, j);
		}
	}

	delete[] mHeightMap;
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
				int index = n*mWidth + m;
				average += mHeightMap[index];
				sample += 1.0f;
			}
		}
	}
	
	return average / sample;
}

bool TerrainClass::inBoundsOfHeightMap(int m, int n)
{
	int index = n*mWidth + m;
	if (index < (mWidth*mHeight) && index > 0)
		return true;

	return false;
}

bool TerrainClass::fillVertexAndIndexData(ID3D11Device* pDevice, WCHAR* texFileName, WCHAR* name2, WCHAR* blendmap)
{
	bool result;
	Vertex* vertices = 0;

	mStride = sizeof(Vertex);
	mVertexCount = mWidth*mHeight;

	
	vertices = new Vertex[mVertexCount]; 

	for (int j = 0; j < mHeight; j++)
	{
		for (int i = 0; i < mWidth; i++)
		{
			int index = j*mWidth + i;
			vertices[index].Pos = XMFLOAT3((i - ((mWidth-1) / 2.0f)), 0.0, (((mHeight-1) / 2.0f) - j));
			vertices[index].Pos.y = mHeightMap[index];	
			vertices[index].texCoord = XMFLOAT2(i / (float)mWidth*4, (float)j / (float)mHeight*4);
			vertices[index].Normal = XMFLOAT3(0, 0, 0);
		}
	}

	int count = (mWidth-1)*(mHeight-1);
	mIndexCount = count * 6;

	unsigned long* indices = new unsigned long[mIndexCount];

	int index = 0;
	for (int j = 0; j < (mHeight - 1); j++)
	{
		for (int i = 0; i < (mWidth - 1); i++)
		{
			int d = j*mWidth + i;
			indices[index] = (unsigned long)d;
			indices[index + 1] = indices[index + 4] = (unsigned long)d + 1;
			indices[index + 2] = indices[index + 3] = (unsigned long)mWidth + d;
			indices[index + 5] = (unsigned long)mWidth + d + 1;
			
			XMVECTOR pos1 = XMLoadFloat3(&(vertices[indices[index]].Pos));
			XMVECTOR pos2 = XMLoadFloat3(&(vertices[indices[index+1]].Pos));
			XMVECTOR pos3 = XMLoadFloat3(&(vertices[indices[index+2]].Pos));

			XMVECTOR vec1 = pos2 - pos1;
			XMVECTOR vec2 = pos3 - pos1;
			
			XMVECTOR norm = XMVector3Cross(vec1, vec2);

			for (int k = 0; k < 3; k++)
			{
				XMVECTOR origNorm = XMLoadFloat3(&(vertices[indices[index + k]].Normal));
				XMVECTOR finNorm = XMVector3Normalize((norm + origNorm));

				XMStoreFloat3(&(vertices[indices[index + k]].Normal), norm);
			}
			XMVECTOR pos4 = XMLoadFloat3(&(vertices[indices[index + 5]].Pos));

			vec1 = pos4 - pos3;
			vec2 = pos2 - pos3;

			norm = XMVector3Cross(vec1, vec2);

			for (int k = 3; k < 6; k++)
			{
				XMVECTOR origNorm = XMLoadFloat3(&(vertices[indices[index + k]].Normal));
				XMVECTOR finNorm = XMVector3Normalize((norm + origNorm));

				XMStoreFloat3(&(vertices[indices[index + k]].Normal), norm);
			}


			index += 6;
		}
	}

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
	
	//tex.push_back(blendmap);

	result = mTexture->Init(pDevice, tex, blendmap);
	if (!result)
	{
		return false;
	}

	return true;
}
