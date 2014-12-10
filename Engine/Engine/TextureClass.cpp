#include "TextureClass.h"
#include "bth_image.h"

TextureClass::TextureClass()
{
	mSRV = 0;
	mTextureCount = 0;
	mUseBlendMap = false;
}


TextureClass::~TextureClass()
{

	for (int i = 0; i < mTextureCount; i++)
	{
		if (mSRV[i])
		{
			mSRV[i]->Release();
			mSRV[i] = 0;
		}
	}
	if (mSRV)
	{
		delete[]mSRV;
		mSRV = 0;
	}
	if (mBlendMapSRV)
	{
		mBlendMapSRV->Release();
		mBlendMapSRV = 0;
	}
}

bool TextureClass::Init(ID3D11Device* pDevice, vector<wstring> fileName, WCHAR* blendMapName)
{

	HRESULT hr;
	mTextureCount = fileName.size();
	
	if (blendMapName)
	{
		mUseBlendMap = true;

		hr = DirectX::CreateWICTextureFromFile(pDevice, blendMapName, 0, &mBlendMapSRV, NULL);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to load blendMap.", blendMapName, MB_OK);
			return false;
		}
		
	}		
	else
		mUseBlendMap = false;

	mSRV = new ID3D11ShaderResourceView*[mTextureCount];
	if (!mSRV)
		return false;

	for (int i = 0; i < mTextureCount; i++)
	{

		hr = DirectX::CreateWICTextureFromFile(pDevice, (WCHAR*)fileName[i].c_str(), 0, &mSRV[i], NULL);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to load texture.", fileName[i].c_str(), MB_OK);
			return false;
		}
	}
	
	
	/*D3D11_TEXTURE2D_DESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.Width = BTH_IMAGE_WIDTH;
	tDesc.Height = BTH_IMAGE_HEIGHT;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.MipLevels = 1;

	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA tbsd;
	ZeroMemory(&tbsd, sizeof(tbsd));
	
	tbsd.pSysMem = (void*)BTH_IMAGE_DATA;
	tbsd.SysMemPitch = BTH_IMAGE_WIDTH * 4 * sizeof(char);

	ID3D11Texture2D * texture;

	hr = pDevice->CreateTexture2D(&tDesc, &tbsd, &texture);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create texture2D.", fileName, MB_OK);
		return false;
	}

	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));

	srvd.Format = tDesc.Format;
	srvd.Texture2D.MipLevels = 1;
	srvd.Texture2D.MostDetailedMip = 0;

	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	pDevice->CreateShaderResourceView(texture, &srvd, &mSRV);
	
	texture->Release();*/

	return true;
}

ID3D11ShaderResourceView** TextureClass::GetShaderResourceView()const
{
	return mSRV;
}

int TextureClass::GetTextureCount()const
{
	return mTextureCount;
}

ID3D11ShaderResourceView* TextureClass::GetBlendMapShaderResourceView()const
{
	return mBlendMapSRV;
}


bool TextureClass::blendEnabled()const
{
	return mUseBlendMap;
}