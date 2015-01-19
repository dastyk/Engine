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
	for (UINT i = 0; i < mParticle.size(); i++)
	{
		delete mParticle[i];
		mParticle[i] = 0;
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

	mParticle.push_back(new ParticleClass(XMFLOAT3(128, 80, 128), XMFLOAT3(1, 0, 0), XMFLOAT3(0,0,0), 0));

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

	for (UINT i = 0; i < mParticle.size(); i++)
	{
		dataPtr[i].Pos = mParticle[i]->GetTranform()->GetPosition();
		dataPtr[i].Color = mParticle[i]->GetColor();
	}


	// Unlock the constant buffer.
	pDeviceContext->Unmap(mVertexBuffer, 0);
}

int ParticleSystemClass::GetAliveParticles()const
{
	return (int)mParticle.size();
}

TextureClass* ParticleSystemClass::GetTexture()const
{
	return mTexture;
}

void ParticleSystemClass::Update(float dt)
{
	for (UINT i = 0; i < mParticle.size(); i++)
	{

		if (mParticle[i]->isAlive())
		{
			mParticle[i]->Update(dt);

		
		}
		else
		{
			delete mParticle[i];
			mParticle.erase(mParticle.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		XMVECTOR dir = XMVectorSet(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50, 0);
		dir = XMVector3Normalize(dir);
		XMFLOAT3 fDir;
		XMStoreFloat3(&fDir, dir);
		mParticle.push_back(new ParticleClass(mParticle[0], fDir, 1));
	}


}

