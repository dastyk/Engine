#include "ShadowMapClass.h"


ShadowMapClass::ShadowMapClass()
{
}


ShadowMapClass::~ShadowMapClass()
{
}

bool Init(ID3D11Device* pDevice)
{

}

bool CreateShadowMap(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera)
{

}

bool InitShader(ID3D11Device*, WCHAR*)
{

}

void RenderShader(ID3D11DeviceContext*, int)
{

}

bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera)
{

}
