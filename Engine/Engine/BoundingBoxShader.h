#ifndef _BOUNDINGBOXSHADER_H_
#define _BOUNDINGBOXSHADER_H_

#pragma once
#include "ShaderClass.h"
class BoundingBoxShader :
	public ShaderClass
{
public:
	BoundingBoxShader();
	~BoundingBoxShader();


	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);



};

#endif