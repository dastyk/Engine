#include "ObjectClass.h"


ObjectClass::ObjectClass()
{
	mModel = nullptr;
	mTransform = nullptr;
	mTime = 0;
	mNrOfFrames = 0;
	mDCount = 0;
}


ObjectClass::ObjectClass(const ObjectClass& other)
{
	this->mModel = other.mModel;
	this->mTransform = new TransformationClass(other.mTransform);
	this->mTime = new GameTimer();
	mTime->Reset();
	mTime->Start();
	mNrOfFrames = other.mNrOfFrames;
	mDCount = 0;
}

ObjectClass::ObjectClass(ModelClass* pModel)
{
	mModel = pModel;
	mTransform = new TransformationClass();
	this->mTime = new GameTimer();
	mTime->Reset();
	mTime->Start();
	if (pModel->GetBoneCount() > 0)
	{
		BoneFrame& b = pModel->GetAnimationClips()[0].bones[0];
		mNrOfFrames = b.frameCount;
	}
	else
		mNrOfFrames = 0;
	mDCount = 0;
}

ObjectClass::ObjectClass(ModelClass* pModel, TransformationClass* pTransform)
{
	mModel = pModel;
	mTransform = pTransform;
	this->mTime = new GameTimer();
	mTime->Reset();
	mTime->Start();
	if (pModel->GetBoneCount() > 0)
	{
		BoneFrame& b = pModel->GetAnimationClips()[0].bones[0];
		mNrOfFrames = b.frameCount;
	}
	else
		mNrOfFrames = 0;
	mDCount = 0;
}


ObjectClass::~ObjectClass()
{
	if (mTransform)
	{
		delete mTransform;
		mTransform = 0;
	}
	if (mTime)
	{
		delete mTime;
		mTime = 0;
	}
}


void ObjectClass::Update()
{
	mTime->Tick();
}


void ObjectClass::SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext, int flag)
{

	mModel->SetAsModelToBeDrawn(pDeviceContext, flag);
}

bool ObjectClass::SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext, BoundingFrustum& frustum, int flag)
{
	XMMATRIX w = XMLoadFloat4x4(&GetWorldMatrix());
	w = XMMatrixInverse(NULL,w);
	BoundingFrustum f;
	frustum.Transform(f, w);
	if (mModel->SetAsModelToBeDrawn(pDeviceContext, f, flag))
		return true;
	else
		return false;
}

XMFLOAT4X4 ObjectClass::GetWorldMatrix() const
{
	return mTransform->GetWorldMatrix();
}

XMFLOAT4X4 ObjectClass::GetNormalMatrix() const
{
	return mTransform->GetNormalMatrix();
}

int ObjectClass::GetIndexCount()const
{
	return mModel->GetIndexCount();
}


TransformationClass* ObjectClass::GetTransformation()const
{
	return mTransform;
}


TextureClass* ObjectClass::GetTexture()const
{
	return mModel->GetTexture();
}

void ObjectClass::SetModel(ModelClass* pModel)
{
	mModel = pModel;
}

MatrialDesc* ObjectClass::GetMaterials()const
{
	return mModel->GetMaterials();
}

ModelClass* ObjectClass::GetModel()const
{
	return mModel;
}


void ObjectClass::interpolateFrames(Frame* f1, Frame* f2, XMFLOAT4X4* m)
{
	float t1 = f1->time;
	float t2 = f2->time;
	

	float lT = 1;// (mTime->TotalTime() - t1) / (t2 - t1);

	XMVECTOR lerpedT;
	XMVECTOR lerpedS;
	XMVECTOR lerpedR;

	XMVECTOR p1 = XMLoadFloat3(&f1->Pos);
	XMVECTOR p2 = XMLoadFloat3(&f2->Pos);
	lerpedT = XMVectorLerp(p1, p2, lT);

	p1 = XMLoadFloat3(&f1->Scale);
	p2 = XMLoadFloat3(&f2->Scale);
	lerpedS = XMVectorLerp(p1, p2, lT);

	p1 = XMLoadFloat4(&f1->Quat);
	p2 = XMLoadFloat4(&f2->Quat);
	lerpedR = XMQuaternionSlerp(p1, p2, lT);

	XMMATRIX T, S, R;

	T = XMMatrixTranslationFromVector(lerpedT);
	S = XMMatrixScalingFromVector(lerpedS);
	R = XMMatrixRotationQuaternion(lerpedR);

	XMStoreFloat4x4(m, R*S*T);

	if (mTime->TotalTime() >= t2)
		mDCount++;
}

void ObjectClass::Animate(XMFLOAT4X4** mBL)
{
	XMFLOAT4X4*& m = (*mBL) = new XMFLOAT4X4[mModel->GetBoneCount()];
	Bone* bones = mModel->GetBones();
	UINT count = mModel->GetBoneCount();
	AnimClipRead* aC = mModel->GetAnimationClips();
	XMFLOAT4X4 mat;
	interpolateFrames(&aC[0].bones[0].Frames[40], &aC[0].bones[0].Frames[40], &mat);
	
	XMMATRIX w = XMMatrixRotationX(0); //XMMatrixRotationQuaternion(XMVectorSet(mTime->TotalTime() / 10.f, 0, 0, 0)); //XMLoadFloat4x4(&mat);//XMMatrixTranslationFromVector(XMVectorSet(mTime->TotalTime(), 0, 0, 0));
	XMMATRIX l = XMLoadFloat4x4(&bones[0].localOffset);
	XMStoreFloat4x4(&bones[0].globalOffset, l*w);

	for (UINT i = 1; i < count; i++)
	{
		w = XMLoadFloat4x4(&bones[bones[i].ParentBone].globalOffset);
		l = XMLoadFloat4x4(&bones[i].localOffset);
		XMStoreFloat4x4(&bones[i].globalOffset, l*w);
	}


	//AnimClipRead* aC = mModel->GetAnimationClips();
	//int time = floorf(mTime->TotalTime());

	//if (mTime->TotalTime() >= 1.25f)
	//{
	//	mTime->Reset();
	//	mTime->Start();
	//	mTime->Tick();
	//	mDCount = 0;
	//}

	//XMFLOAT4X4 mat;
	//interpolateFrames(&aC[0].bones[0].Frames[mDCount], &aC[0].bones[0].Frames[mDCount + 1], &mat);
	//XMMATRIX w = XMLoadFloat4x4(&mat);
	//XMMATRIX l = XMLoadFloat4x4(&bones[0].localOffset);
	//XMStoreFloat4x4(&bones[0].globalOffset, l*w);

	//for (UINT i = 1; i < count; i++)
	//{
	//	//interpolateFrames(&aC[0].bones[0].Frames[time], &aC[0].bones[0].Frames[time + 1], &mat);
	//	w = XMLoadFloat4x4(&bones[bones[i].ParentBone].globalOffset); //w = XMLoadFloat4x4(&mat);
	//	l = XMLoadFloat4x4(&bones[i].localOffset);
	//	XMStoreFloat4x4(&bones[i].globalOffset, l*w);
	//}



	for (UINT i = 0; i < count; i++)
	{
		XMMATRIX mat = XMLoadFloat4x4(&bones[i].globalOffset);
		XMMATRIX BP = XMLoadFloat4x4(&bones[i].invBindPose);
		XMStoreFloat4x4(&m[i], XMMatrixTranspose(BP*mat));
	}




	//XMMATRIX w = XMLoadFloat4x4(&mat);
	//XMMATRIX bP = XMLoadFloat4x4(&bones[0].invBindPose);
	//XMMATRIX fin;
	//XMStoreFloat4x4(&bones[0].globalOffset, w);
	//XMStoreFloat4x4(&m[0], XMMatrixTranspose(bP*w));

	//

	//for (UINT i = 1; i < count; i++)
	//{
	//	Bone& b = bones[i];
	//	interpolateFrames(&aC[0].bones[i].Frames[time], &aC[0].bones[i].Frames[time + 1], &mat);
	//	w = XMLoadFloat4x4(&mat);
	//	XMMATRIX bP = XMLoadFloat4x4(&b.invBindPose);
	//	XMMATRIX g = XMLoadFloat4x4(&bones[b.ParentBone].globalOffset);
	//	fin = w*g;
	//	XMStoreFloat4x4(&b.globalOffset, fin);
	//	XMStoreFloat4x4(&m[i], XMMatrixTranspose(bP*fin));
	//}
}

BoundingBox ObjectClass::GetBoundingBox()const
{
	BoundingBox b = mModel->GetBoundingBox();

	XMMATRIX w = XMLoadFloat4x4(&mTransform->GetWorldMatrix());

	b.Transform(b, w);

	return b;
}