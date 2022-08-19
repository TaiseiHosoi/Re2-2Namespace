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

	void UpdateMatrixWorld(Matrix4& matTrans, Matrix4& matRotateY,WorldTransform& worldTransform_);

	void VectorUpdate(Vector3& vec, WorldTransform& worldTransform_);

};

