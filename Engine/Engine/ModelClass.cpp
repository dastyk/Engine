#include "ModelClass.h"


ModelClass::ModelClass()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mTexture = 0;
	mVertexCount = 0;
	mIndexCount = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}



ModelClass::~ModelClass()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}

	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}

	if (mTexture)
	{
		delete mTexture;
		mIndexBuffer = 0;
	}
}


void ModelClass::SetAsModelToBeDrawn(ID3D11DeviceContext* pDeviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = mStride;
	offset = 0;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.

	pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	pDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return;
}



int ModelClass::GetIndexCount() const
{
	return (int)mIndexCount;
}


bool ModelClass::createModel(ID3D11Device* pDevice, char* modelName, WCHAR* texFileName)
{
	//bool result;

	//struct Vertex
	//{
	//	XMFLOAT3 Pos;
	//	XMFLOAT2 texCoord;
	//};

	//mStride = sizeof(Vertex);

	//
	//// Create Vertex Buffer
	//mVertexCount = 24;
	//Vertex vertices[] =
	//{
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0, 1.0) },

	//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0, 1.0) },
	//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0, 1.0) },

	//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0, 0.0) },
	//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0, 0.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0, 1.0) },

	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0, 1.0) },
	//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0, 1.0) },

	//	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0, 1.0) },

	//	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0, 1.0) },
	//	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0, 0.0) },
	//	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0, 1.0) }

	//};


	//D3D11_SUBRESOURCE_DATA vinitData;
	//vinitData.pSysMem = vertices;
	//vinitData.SysMemPitch = 0;
	//vinitData.SysMemSlicePitch = 0;

	//result = createVertexBuffer(pDevice, &vinitData, mStride * mVertexCount);
	//if (!result)
	//{
	//	return false;
	//}

	//mIndexCount = 36;
	//unsigned long indices[]
	//{
	//	0, 1, 2,
	//		0, 2, 3,

	//		4, 6, 5,
	//		4, 7, 6,

	//		8, 9, 10,
	//		8, 10, 11,

	//		12, 13, 14,
	//		12, 14, 15,

	//		16, 17, 18,
	//		16, 18, 19,

	//		20, 22, 21,
	//		20, 23, 22
	//};

	//D3D11_SUBRESOURCE_DATA iinitData;
	//iinitData.pSysMem = indices;
	//iinitData.SysMemPitch = 0;
	//iinitData.SysMemSlicePitch = 0;


	//result = createIndexBuffer(pDevice, &iinitData, sizeof(unsigned long)*mIndexCount);
	//if (!result)
	//{
	//	return false;
	//}


	//mTexture = new TextureClass();
	//if (!mTexture)
	//{
	//	return false;
	//}

	//result = mTexture->Init(pDevice,texFileName);
	//if (!result)
	//{
	//	return false;
	//}


bool result;

	

	mStride = sizeof(Vertex);

	
	// Create Vertex Buffer
	/*mVertexCount = 24;
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0, 2.0), XMFLOAT3(0,0,-1)},
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0, 0.0), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(2.0, 0.0), XMFLOAT3(0, 0, -1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(2.0, 2.0), XMFLOAT3(0, 0, -1) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(2.0, 2.0), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(2.0, 0.0), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0, 0.0), XMFLOAT3(0, 0, 1) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0, 2.0), XMFLOAT3(0, 0, 1) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0, 2.0), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0, 0.0), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(2.0, 0.0), XMFLOAT3(-1, 0, 0) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(2.0, 2.0), XMFLOAT3(-1, 0, 0) },

		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0, 2.0), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0, 0.0), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(2.0, 0.0), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(2.0, 2.0), XMFLOAT3(1, 0, 0) },

		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0, 2.0), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0, 0.0), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(2.0, 0.0), XMFLOAT3(0, 1, 0) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(2.0, 2.0), XMFLOAT3(0, 1, 0) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0, 2.0), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0, 0.0), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(2.0, 0.0), XMFLOAT3(0, -1, 0) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(2.0, 2.0), XMFLOAT3(0, -1, 0) }

	};*/

	Vertex* vertices;
	unsigned long* indices;
	LoadModel(modelName, mVertexCount, &vertices, mIndexCount, &indices);


	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;

	result = createVertexBuffer(pDevice, &vinitData, mStride * mVertexCount);
	if (!result)
	{
		return false;
	}

	/*mIndexCount = 36;
	unsigned long indices[]
	{
		0, 1, 2,
			0, 2, 3,

			4, 6, 5,
			4, 7, 6,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 22, 21,
			20, 23, 22
	};*/

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	iinitData.SysMemPitch = 0;
	iinitData.SysMemSlicePitch = 0;


	result = createIndexBuffer(pDevice, &iinitData, sizeof(unsigned long)*mIndexCount);
	if (!result)
	{
		return false;
	}

	delete[] vertices;
	delete[] indices;

	mTexture = new TextureClass();
	if (!mTexture)
	{
		return false;
	}

	vector<wstring> tex;
	tex.push_back(texFileName);

	result = mTexture->Init(pDevice,tex,NULL);
	if (!result)
	{
		return false;
	}


	return true;
}

bool ModelClass::createVertexBuffer(ID3D11Device* pDevice, D3D11_SUBRESOURCE_DATA* pData, UINT byteWidth)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = byteWidth;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&vbd, pData, &mVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Vertex Buffer.", 0, 0);
		return false;
	}
	
	return true;
}

bool ModelClass::createVertexBuffer(ID3D11Device* pDevice, ID3D11Buffer **ppBuffer, UINT byteWidth)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = byteWidth;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&vbd, NULL, ppBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create dynamic Vertex Buffer.", 0, 0);
		return false;
	}
	
	return true;
}
bool ModelClass::createIndexBuffer(ID3D11Device* pDevice, D3D11_SUBRESOURCE_DATA* pData, UINT byteWidth)
{
	HRESULT hr;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = byteWidth;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&ibd, pData, &mIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Index Buffer.", 0, 0);
		return false;
	}
	return true;
}

bool ModelClass::createIndexBuffer(ID3D11Device* pDevice, ID3D11Buffer **ppBuffer, UINT byteWidth)
{
	HRESULT hr;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = byteWidth;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&ibd, NULL, ppBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create dynamic Index Buffer.", 0, 0);
		return false;
	}
	return true;
}

TextureClass* ModelClass::GetTexture()const
{
	return mTexture;
}


bool ModelClass::SetAsModelToBeDrawn(ID3D11DeviceContext* pDeviceContext, BoundingFrustum& frustum)
{
	return false;
}