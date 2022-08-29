#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DirectXCommon.h"
#include "Math.h"
#include"Matrix4.h"
#include<Vector4.h>

namespace Affin
{

	void init();

	void UpdateTrans(Matrix4& matTrans, WorldTransform& worldTransform_);

	void UpdateScale(Matrix4& matScale,WorldTransform& worldTransform_);

	void UpdateRotate(Matrix4& matRotateZ ,WorldTransform& worldTransform_);

	void UpdateRotateY(Matrix4& matRotateY, WorldTransform& worldTransform_);

	void UpdateMatrixWorld(Matrix4& matScale,Matrix4& matTrans, Matrix4& matRotateY,WorldTransform& worldTransform_);

	void VectorUpdate(Vector3& vec, WorldTransform& worldTransform_);

	void VectorTransUpdate(Vector3& vec, WorldTransform& worldTransform_);

	void MatVec(Vector3& vector3, WorldTransform& worldTransform_);

	Vector3 GetVecTrans(Matrix4& mat);

	Matrix4 GetMatTrans(Vector3& vec);

	void AffinUpdate(WorldTransform& worldTransform);	//worldtransformのローカル値で更新が完結できる

	Vector3 AddVector(const Vector3& vec1, const Vector3& vec2);

	Vector3 WDiv(Vector3& vec, Matrix4& Mat);
};

