#include "Affin.h"
#include"input/input.h"
#include"cmath"






void Affin::UpdateTrans(Matrix4& matTrans, WorldTransform& worldTransform_)
{
	matTrans = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z,1.0f
	};

	worldTransform_.matWorld_ *= matTrans;

}

void Affin::UpdateScale(Matrix4& matScale, WorldTransform& worldTransform_)
{
	matScale = {
			worldTransform_.scale_.x,0.0f,0.0f,0.0f,
			0.0f,worldTransform_.scale_.y,0.0f,0.0f,
			0.0f,0.0f,worldTransform_.scale_.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
	};

	worldTransform_.matWorld_ *= matScale;



}

void Affin::UpdateRotate(Matrix4& matRotateZ, WorldTransform& worldTransform_)
{
	matRotateZ = {
		cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0.0f,0.0f,
		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	worldTransform_.matWorld_ *= matRotateZ;



}
