#pragma once
#include "BaseScene.h"

//�ݒ��ʃN���X
class Config : public BaseScene{

public:
	Config(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;		//�������������I�[�o�[���C�h
	void Finalize() override;		//�I���������I�[�o�[���C�h
	void Update() override;			//�X�V�������I�[�o�[���C�h
	void Draw() override;			//�`�揈�����I�[�o�[���C�h
};

void Config_Initialize();