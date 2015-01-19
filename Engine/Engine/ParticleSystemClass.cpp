#include "ParticleSystemClass.h"


ParticleSystemClass::ParticleSystemClass()
{
	mVertexBuffer = nullptr;
	mVertexCount = 0;
	mTexture = 0;
}


ParticleSystemClass::~ParticleSystemClass()
{
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}
	for (UINT i = 0; i < mConstant.size(); i++)
	{
		delete mConstant[i];
		mConstant[i] = 0;
	}
	for (UINT i = 0; i < mEmitters.size(); i++)
	{
		delete mEmitters[i];
		mEmitters[i] = 0;
	}
	for (UINT i = 0; i < mMoving.size(); i++)
	{
		delete mMoving[i];
		mMoving[i] = 0;
	}
	if (mTexture)
	{
		delete mTexture;
		mTexture = 0;
	}
}

bool ParticleSystemClass::Init(ID3D11Device* pDevice)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(ParticleVertex) * 999999;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&vbd, NULL, &mVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create dynamic Vertex Buffer.", 0, 0);
		return false;
	}

	mTexture = new TextureClass();
	if (!mTexture)
	{
		return false;
	}

	vector<wstring> tex;
	tex.push_back(L"data/resources/BTH_ny.jpg");

	bool result = mTexture->Init(pDevice, tex, NULL);
	if (!result)
	{
		return false;
	}

	createFirstParticles();

	/* //Create Vertex Buffer
	mVertexCount = 4;
	ParticleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(1,0,0) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(1, 0, 0) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1, 0, 0) }
	};


	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;

	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(ParticleVertex)*mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Vertex Buffer.", 0, 0);
		return false;
	}*/

	return true;
}


void ParticleSystemClass::render(ID3D11DeviceContext* pDeviceContext)
{

	LoadParticlesToBuffer(pDeviceContext);

	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(ParticleVertex);
	offset = 0;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.

	pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// Set topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void ParticleSystemClass::LoadParticlesToBuffer(ID3D11DeviceContext* pDeviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ParticleVertex* dataPtr;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (ParticleVertex*)mappedResource.pData;

	UINT offset = 0;

	for (UINT i = 0; i < mConstant.size(); i++)
	{
		dataPtr[offset].Pos = mConstant[i]->GetTranform()->GetPosition();
		dataPtr[offset].Color = mConstant[i]->GetColor();
		offset++;
	}	
	for (UINT i = 0; i < mEmitters.size(); i++)
	{
		dataPtr[offset].Pos = mEmitters[i]->GetTranform()->GetPosition();
		dataPtr[offset].Color = mEmitters[i]->GetColor();
		offset++;
	}
	for (UINT i = 0; i < mMoving.size(); i++)
	{
		dataPtr[offset].Pos = mMoving[i]->GetTranform()->GetPosition();
		dataPtr[offset].Color = mMoving[i]->GetColor();
		offset++;
	}

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mVertexBuffer, 0);
}

int ParticleSystemClass::GetAliveParticles()const
{
	return  (int)mConstant.size() + mMoving.size() + mEmitters.size();
}

TextureClass* ParticleSystemClass::GetTexture()const
{
	return mTexture;
}

void ParticleSystemClass::Update(float dt)
{
	UINT count = mConstant.size();
	for (UINT i = 0; i < count; i++)
	{

			mConstant[i]->Update(dt);
			CreateConstantInUpdate(mConstant[i]);

	}

	count = mMoving.size();
	for (UINT i = 0; i < count; i++)
	{

		if (mMoving[i]->isAlive())
		{
			mMoving[i]->Update(dt);
			CreateMovingInUpdate(mMoving[i]);

		}
		else
		{
			delete mMoving[i];
			mMoving.erase(mMoving.begin() + i);
			i--;
			count--;
		}

	}

	count = mEmitters.size();
	for (UINT i = 0; i < count; i++)
	{

		if (mEmitters[i]->isAlive())
		{
			mEmitters[i]->Update(dt);
			CreateEmitterInUpdate(mEmitters[i]);

		}
		else
		{
			delete mEmitters[i];
			mEmitters.erase(mEmitters.begin() + i);
			i--;
			count--;
		}

	}

}

