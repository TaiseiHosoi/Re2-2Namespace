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

void Affin::UpdateMatrixWorld(Matrix4 &matScale,Matrix4 &matTrans, Matrix4 &matRotateY,WorldTransform& worldTransform_)
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();	
	worldTransform_.matWorld_ *= matRotateY;
	worldTransform_.matWorld_ *= matTrans;
	worldTransform_.matWorld_ *= matScale;
	

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
	//worldTransform_.matWorld_ *= matVec;
	

	vec.x = matVec.m[3][0];
	vec.y = matVec.m[3][1];
	vec.z = matVec.m[3][2];



}

void Affin::VectorTransUpdate(Vector3& vec, WorldTransform& worldTransform_)
{
	Matrix4 matV = Affin::GetMatTrans(vec);
	
	matV *= worldTransform_.matWorld_;

	vec.x = matV.m[3][0];
	vec.y = matV.m[3][1];
	vec.z = matV.m[3][2];
	
}

void Affin::MatVec(Vector3& vector3, WorldTransform& worldTransform)
{
	Vector3 matVector = { 0,0,0 };

	matVector.x = vector3.x * worldTransform.matWorld_.m[0][0] + vector3.y * worldTransform.matWorld_.m[1][0] + vector3.z * worldTransform.matWorld_.m[2][0];
	matVector.y = vector3.x * worldTransform.matWorld_.m[0][1] + vector3.y * worldTransform.matWorld_.m[1][1] + vector3.z * worldTransform.matWorld_.m[2][1];
	matVector.z = vector3.x * worldTransform.matWorld_.m[0][2] + vector3.y * worldTransform.matWorld_.m[1][2] + vector3.z * worldTransform.matWorld_.m[2][2];

	vector3 = matVector;
}

Vector3 Affin::GetVecTrans(Matrix4& mat)
{
	Vector3 vec = { 0,0,0 };
	vec.x = mat.m[3][0];
	vec.y = mat.m[3][1];
	vec.z = mat.m[3][2];

	return vec;
}






Matrix4 Affin::GetMatTrans(Vector3& vec)
{

	Matrix4 mat = MathUtility::Matrix4Identity();

	mat.m[3][0] = vec.x;
	mat.m[3][1] = vec.y;
	mat.m[3][2] = vec.z;
	return mat;
}

void Affin::AffinUpdate(WorldTransform& worldTransform)
{
	Matrix4 mat = MathUtility::Matrix4Identity();

	Matrix4 matRotY= {
	cos(worldTransform.rotation_.y),0,sin(worldTransform.rotation_.y),0.0f,
	0.0f,1.0f,0.0f,0.0f,
	-sin(worldTransform.rotation_.y),0.0f,cos(worldTransform.rotation_.y),0.0f,
	0.0f,0.0f,0.0f,1.0f
	};

	Matrix4 matRotZ = {
		cos(worldTransform.rotation_.z),sin(worldTransform.rotation_.z),0.0f,0.0f,
		-sin(worldTransform.rotation_.z),cos(worldTransform.rotation_.z),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	Matrix4 matScale = {
			worldTransform.scale_.x,0.0f,0.0f,0.0f,
			0.0f,worldTransform.scale_.y,0.0f,0.0f,
			0.0f,0.0f,worldTransform.scale_.z,0.0f,
			0.0f,0.0f,0.0f,1.0f
	};

	Matrix4 matTrans = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z,1.0f
	};

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform.matWorld_ *= matRotY;
	worldTransform.matWorld_ *= matRotZ;
	worldTransform.matWorld_ *= matTrans;
	worldTransform.matWorld_ *= matScale;

}

Vector3 Affin::AddVector(const Vector3& vec1, const Vector3& vec2)
{
	Vector3 vec;

	vec.x = vec1.x + vec2.x;
	vec.y = vec1.y + vec2.y;
	vec.z = vec1.z + vec2.z;

	return vec;
}

Vector3 Affin::WDiv(Vector3& vec, Matrix4& Mat)
{
	Vector4 div = {0,0,0,0};

	div.x = vec.x * Mat.m[0][0] + vec.y * Mat.m[1][0] + vec.z * Mat.m[2][0] + 1 * Mat.m[3][0];
	div.y = vec.x * Mat.m[0][1] + vec.y * Mat.m[1][1] + vec.z * Mat.m[2][1] + 1 * Mat.m[3][1];
	div.z = vec.x * Mat.m[0][2] + vec.y * Mat.m[1][2] + vec.z * Mat.m[2][2] + 1 * Mat.m[3][2];
	div.w = vec.x * Mat.m[0][3] + vec.y * Mat.m[1][3] + vec.z * Mat.m[2][3] + 1 * Mat.m[3][3];

	div.x /= div.w;
	div.y /= div.w;
	div.z /= div.w;

	return Vector3(div.x, div.y, div.z);

}


