#include "TextureClass.h"
#include "bth_image.h"

TextureClass::TextureClass()
{
	mSRV = 0;
	mTexture = 0;
	mTextureCount = 0;
	mUseBlendMap = false;
}


TextureClass::~TextureClass()
{

	if (mSRV)
	{
		for (int i = 0; i < mTextureCount; i++)
		{
			if (mSRV[i])
			{
				mSRV[i]->Release();
				mSRV[i] = 0;
			}
		}
		delete[]mSRV;
		mSRV = 0;
	}
	if (mTexture)
	{
		for (int i = 0; i < mTextureCount; i++)
		{
			if (mTexture[i])
			{
				mTexture[i]->Release();
				mTexture[i] = 0;
			}
		}
		delete[]mTexture;
		mTexture = 0;
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
	mTextureCount = fileName.size();
	mSRV = new ID3D11ShaderResourceView*[mTextureCount];
	if (!mSRV)
		return false;

	for (int i = 0; i < mTextureCount; i++)
	{
		mSRV[i] = 0;
	}


	mTexture = new ID3D11Texture2D*[mTextureCount];
	if (!mTexture)
		return false;

	for (int i = 0; i < mTextureCount; i++)
	{
		mTexture[i] = 0;
	}

	for (int i = 0; i < mTextureCount; i++)
	{
		if (fileName[i].c_str()[0] == '.')
			fileName[i].erase(0, 2);
		if (GetExtension(fileName[i]) == wstring(L"tga"))
		{
			fileName[i] = removeExtension(fileName[i]) + L".jpg";
		}
		fileName[i] = L"data/resources/" + fileName[i];
		hr = DirectX::CreateWICTextureFromFileEx(pDevice, (WCHAR*)fileName[i].c_str(), NULL, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, (ID3D11Resource**)&mTexture[i], NULL);//DirectX::CreateWICTextureFromFile(pDevice, (WCHAR*)fileName[i].c_str(), (ID3D11Resource**)&mTexture[i], NULL, NULL);
		
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to load texture.", fileName[i].c_str(), MB_OK);
			return false;
		}
		
		D3D11_TEXTURE2D_DESC tDesc;
		ZeroMemory(&tDesc, sizeof(tDesc));
		mTexture[i]->GetDesc(&tDesc);



		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));

		srvd.Format = tDesc.Format;
		srvd.Texture2D.MipLevels = tDesc.MipLevels;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		hr = pDevice->CreateShaderResourceView(mTexture[i], &srvd, &mSRV[i]);

		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to create SRV.", fileName[i].c_str(), MB_OK);
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

std::wstring TextureClass::GetExtension(const std::wstring& filename)
{
	std::string::size_type idx;

	idx = filename.rfind('.');

	if (idx != std::wstring::npos)
	{
		return filename.substr(idx + 1);
	}

	return L"";

}

std::wstring TextureClass::removeExtension(const std::wstring& filename) {
	size_t lastdot = filename.find_last_of(L".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}
