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

}

void Affin::UpdateScale(Matrix4& matScale, WorldTransform& worldTransform_)
{
	matScale = {
			worldTransform_.scale_.x,0.0f,0.0f,0.0f,
			0.0f,worldTransform_.scale_.y,0.0f,0.0f,
			0.0f,0.0f,worldTransform_.scale_.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
	};

}

void Affin::UpdateRotate(Matrix4& matRotateZ, WorldTransform& worldTransform_)
{
	matRotateZ = {
		cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0.0f,0.0f,
		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};




}

void Affin::UpdateRotateY(Matrix4& matRotateY, WorldTransform& worldTransform_)
{
	
	matRotateY = {
		cos(worldTransform_.rotation_.y),0,sin(worldTransform_.rotation_.y),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		-sin(worldTransform_.rotation_.y),0.0f,cos(worldTransform_.rotation_.y),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

}

void Affin::UpdateMatrixWorld(Matrix4 &matScale ,Matrix4 &matTrans, Matrix4 &matRotateY,WorldTransform& worldTransform_)
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();	
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRotateY;
	worldTransform_.matWorld_ *= matTrans;
	

}

void Affin::VectorUpdate(Vector3& vec ,WorldTransform& worldTransform_)
{
	Matrix4 matVec = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		vec.x,vec.y,vec.z,1.0f
	};

	Matrix4 matRotateY = {
		cos(worldTransform_.rotation_.y),0,sin(worldTransform_.rotation_.y),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		-sin(worldTransform_.rotation_.y),0.0f,cos(worldTransform_.rotation_.y),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	matVec *= matRotateY;

	vec.x = matVec.m[3][0];
	vec.y = matVec.m[3][1];
	vec.z = matVec.m[3][2];



}


