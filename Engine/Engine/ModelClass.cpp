#include "ModelClass.h"


ModelClass::ModelClass()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mTexture = 0;
	mVertexCount = 0;
	mIndexCount = 0;
	mObjectCount = 0;
	mMaterial = 0;
	mBoneCount = 0;
	mAnimationClipCount = 0;
	mBones = 0;
	mAnimationClips = 0;
	mBox = 0;
	mBoundingBoxVBuffer = 0;
	mBoundingBoxIBuffer = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}



ModelClass::~ModelClass()
{
	if (mBoundingBoxIBuffer)
	{
		mBoundingBoxIBuffer->Release();
		mBoundingBoxIBuffer = 0;
	}
	if (mBoundingBoxVBuffer)
	{
		mBoundingBoxVBuffer->Release();
		mBoundingBoxVBuffer = 0;
	}
	if (mBox)
	{
		delete mBox;
		mBox = 0;
	}
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
	if (mMaterial)
	{
		delete[] mMaterial;
		mMaterial = 0;
	}
	if (mBones)
	{
		delete[] mBones;
		mBones = 0;
	}
	if (mAnimationClips)
	{
		for (UINT i = 0; i < mAnimationClipCount; i++)
		{
			if (mAnimationClips[i].bones)
			{
				for (UINT j = 0; j < mBoneCount; j++)
				{
					if (mAnimationClips[i].bones[j].Frames)
					{
						delete[] mAnimationClips[i].bones[j].Frames;
						mAnimationClips[i].bones[j].Frames = 0;
					}
				}
				delete[]mAnimationClips[i].bones;
				mAnimationClips[i].bones = 0;
			}
		}
		delete[] mAnimationClips;
		mAnimationClips = 0;
	}
}


void ModelClass::SetAsModelToBeDrawn(ID3D11DeviceContext* pDeviceContext, int flag)
{
	unsigned int stride;
	unsigned int offset;
	if (flag == 0)
	{
		// Set vertex buffer stride and offset.
		stride = mStride;
		offset = 0;

		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.

		pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		pDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	}
	else if (flag == -1)
	{
		// Set vertex buffer stride and offset.
		stride = mStride;
		offset = 0;

		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.

		pDeviceContext->IASetVertexBuffers(0, 1, &mBoundingBoxVBuffer, &stride, &offset);

		pDeviceContext->IASetIndexBuffer(mBoundingBoxIBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}


	
	return;
}



UINT ModelClass::GetIndexCount() const
{
	return (UINT)mIndexCount;
}


bool ModelClass::createModel(ID3D11Device* pDevice, char* modelName)
{


bool result;

	

	mStride = sizeof(Vertex);

	
	Vertex* vertices = nullptr;
	unsigned long* indices = nullptr;
	vector<wstring> tex;
	BoneRead* bones;


	LoadSmfModel(modelName, mVertexCount, &vertices, mIndexCount, &indices, mObjectCount, tex, &mMaterial, &bones, mBoneCount, &mAnimationClips, mAnimationClipCount);


	mBox = new AABB;
	mBox->createFromPoints(mVertexCount, (XMFLOAT3*)vertices, sizeof(Vertex));


	if (mBoneCount > 0)
	{
		mBones = new Bone[mBoneCount];
		mBones[0].ParentBone = bones[0].ParentBone;
		mBones[0].localOffset = mBones[0].globalOffset = bones[0].localOffset;			
		XMMATRIX m = XMLoadFloat4x4(&mBones[0].globalOffset);
		XMStoreFloat4x4(&mBones[0].invBindPose, XMMatrixInverse(&XMVectorSet(0,0,0,0),m));

		for (UINT i = 1; i < mBoneCount; i++)
		{
			Bone& b = mBones[i];
			BoneRead& r = bones[i];
			b.ParentBone = r.ParentBone;
			b.localOffset = r.localOffset;
			XMMATRIX Pg = XMLoadFloat4x4(&mBones[b.ParentBone].globalOffset);
			XMMATRIX g = XMLoadFloat4x4(&b.localOffset);
			XMStoreFloat4x4(&b.globalOffset, g*Pg);
			XMMATRIX m = XMLoadFloat4x4(&mBones[i].globalOffset);
			XMStoreFloat4x4(&mBones[i].invBindPose, XMMatrixInverse(&XMVectorSet(0, 0, 0, 0), m));
			
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





	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	iinitData.SysMemPitch = 0;
	iinitData.SysMemSlicePitch = 0;


	result = createIndexBuffer(pDevice, &iinitData, sizeof(unsigned long)*mIndexCount);
	if (!result)
	{
		return false;
	}


	BoundingBox b = mBox->GetBoundingBox();
	XMFLOAT3 c = b.Center;
	XMFLOAT3 e = b.Extents;

	//BoundingVertex d[] =
	//{
	//	{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(1, 0, 0) },
	//	{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(1, 0, 0) },
	//	{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(1, 0, 0) },

	//	{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(1, 0, 0) },
	//	{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(1, 0, 0) },
	//	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(1, 0, 0) }


	//};
	//BoundingVertex vData[] = 
	//{
	//	// Front face	
	//	{ XMFLOAT3(c.x - e.x, c.y - e.y, c.z - e.z), XMFLOAT3(1, 0, 0) }, // 0 bottom left front
	//	{ XMFLOAT3(c.x - e.x, c.y + e.y, c.z - e.z), XMFLOAT3(1, 0, 0) },// 1 top left front
	//	{ XMFLOAT3(c.x + e.x, c.y + e.y, c.z - e.z), XMFLOAT3(1, 0, 0) },// 2 top right front
	//	{ XMFLOAT3(c.x + e.x, c.y - e.y, c.z - e.z), XMFLOAT3(1, 0, 0) },// 3 bottom right front

	//	//// BackFace
	//	{ XMFLOAT3(c.x - e.x, c.y - e.y, c.z + e.z), XMFLOAT3(1, 0, 0) },// 4 bottom left back
	//	{ XMFLOAT3(c.x - e.x, c.y + e.y, c.z + e.z), XMFLOAT3(1, 0, 0) },// 5 top left back
	//	{ XMFLOAT3(c.x + e.x, c.y + e.y, c.z + e.z), XMFLOAT3(1, 0, 0) },// 6 top right back
	//	{ XMFLOAT3(c.x + e.x, c.y - e.y, c.z + e.z), XMFLOAT3(1, 0, 0) }// 7 bottom right back

	//};

	BoundingVertex vData[] =
	{
		// Front face	
		{ XMFLOAT3(c.x - e.x, c.y - e.y, c.z - e.z)}, // 0 bottom left front
		{ XMFLOAT3(c.x - e.x, c.y + e.y, c.z - e.z)},// 1 top left front
		{ XMFLOAT3(c.x + e.x, c.y + e.y, c.z - e.z)},// 2 top right front
		{ XMFLOAT3(c.x + e.x, c.y - e.y, c.z - e.z)},// 3 bottom right front

		//// BackFace
		{ XMFLOAT3(c.x - e.x, c.y - e.y, c.z + e.z)},// 4 bottom left back
		{ XMFLOAT3(c.x - e.x, c.y + e.y, c.z + e.z)},// 5 top left back
		{ XMFLOAT3(c.x + e.x, c.y + e.y, c.z + e.z)},// 6 top right back
		{ XMFLOAT3(c.x + e.x, c.y - e.y, c.z + e.z)}// 7 bottom right back

	};


	unsigned long iData[] = {
		0, 1, 3,
		3, 1, 2,

		1, 5, 6,
		1, 6, 2,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		7, 6, 5,
		7, 5, 4,

		7, 3, 0,
		7, 0, 4


	};


	D3D11_SUBRESOURCE_DATA vDat;
	vDat.pSysMem = vData;
	vDat.SysMemPitch = 0;
	vDat.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA iDat;
	iDat.pSysMem = iData;
	iDat.SysMemPitch = 0;
	iDat.SysMemSlicePitch = 0;


	HRESULT hr;
	D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BoundingVertex)*8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&vbd, &vDat, &mBoundingBoxVBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Vertex Buffer.", 0, 0);
		return false;
	}


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned long)* 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&ibd, &iDat, &mBoundingBoxIBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Index Buffer.", 0, 0);
		return false;
	}

	delete[]bones;
	delete[] vertices;
	delete[] indices;

	mTexture = new TextureClass();
	if (!mTexture)
	{
		return false;
	}

	

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

bool ModelClass::createVertexBuffer(ID3D11Device* pDevice, D3D11_SUBRESOURCE_DATA* pData, ID3D11Buffer **ppBuffer, UINT byteWidth)
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

	hr = pDevice->CreateBuffer(&vbd, pData, ppBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Vertex Buffer.", 0, 0);
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

bool ModelClass::createIndexBuffer(ID3D11Device* pDevice, D3D11_SUBRESOURCE_DATA* pData, ID3D11Buffer **ppBuffer, UINT byteWidth)
{
	HRESULT hr;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = byteWidth;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&ibd, pData, ppBuffer);
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


bool ModelClass::SetAsModelToBeDrawn(ID3D11DeviceContext* pDeviceContext, BoundingFrustum& frustum, int flag)
{
	if (flag == -1)
	{
		if (frustum.Contains(mBox->GetBoundingBox()) > 0)
		{
			unsigned int stride;
			unsigned int offset;

			// Set vertex buffer stride and offset.
			stride = mStride;
			offset = 0;

			// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.

			pDeviceContext->IASetVertexBuffers(0, 1, &mBoundingBoxVBuffer, &stride, &offset);

			pDeviceContext->IASetIndexBuffer(mBoundingBoxIBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set topology
			pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			return true;
		}
	}
	else
	{
		if (frustum.Contains(mBox->GetBoundingBox()) > flag)
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

			return true;
		}
	}
	return false;
}

MatrialDesc* ModelClass::GetMaterials()const
{
	return mMaterial;
}

UINT ModelClass::GetObjectCount()const
{
	return mObjectCount;
}

UINT ModelClass::GetBoneCount()const
{
	return mBoneCount;
}

Bone* ModelClass::GetBones()const
{
	return mBones;
}

UINT ModelClass::GetAnimationClipCount()const
{
	return mAnimationClipCount;
}

AnimClipRead* ModelClass::GetAnimationClips()const
{
	return mAnimationClips;
}
