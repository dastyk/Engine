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

	if (mBoneCount > 0)
	{
		mBones = new Bone[mBoneCount];
		mBones[0].ParentBone = bones[0].ParentBone;
		mBones[0].localOffset = mBones[0].globalOffset = bones[0].localOffset;		
		for (UINT i = 0; i < mBoneCount; i++)
		{
			Bone& b = mBones[i];
			BoneRead& r = bones[i];
			b.ParentBone = r.ParentBone;
			b.localOffset = r.localOffset;
			XMMATRIX Pg = XMLoadFloat4x4(&mBones[b.ParentBone].globalOffset);
			XMMATRIX g = XMLoadFloat4x4(&b.localOffset);
			XMStoreFloat4x4(&b.globalOffset, Pg*g);
			
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

MatrialDesc* ModelClass::GetMaterials()const
{
	return mMaterial;
}

UINT ModelClass::GetObjectCount()const
{
	return mObjectCount;
}