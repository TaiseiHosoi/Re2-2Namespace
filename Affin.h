#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DirectXCommon.h"
#include "Math.h"
#include"Matrix4.h"
namespace Affin
{

	void init();

	void UpdateTrans(Matrix4& matTrans, WorldTransform& worldTransform_);

	void UpdateScale(Matrix4& matScale,WorldTransform& worldTransform_);

	
	void UpdateRotate(Matrix4& matRotateZ ,WorldTransform& worldTransform_);

	




};

