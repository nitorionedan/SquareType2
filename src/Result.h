#pragma once
#include "BaseScene.h"

class Result : public BaseScene{

public:
	Result(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;		//�������������I�[�o�[���C�h
	void Finalize() override;		//�I���������I�[�o�[���C�h
	void Update() override;			//�X�V�������I�[�o�[���C�h
	void Draw() override;			//�`�揈�����I�[�o�[���C�h
};