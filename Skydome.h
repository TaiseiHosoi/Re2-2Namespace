#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
/// <summary>
/// �V��
/// </summary>
class Skydome
{
public
	

	void Initialize();

	void Update();

	void Draw(ViewProjection viewProjection);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	
};

