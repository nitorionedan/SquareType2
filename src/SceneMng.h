#pragma once

#include "ISceneChanger.h"
#include "BaseScene.h"

class SceneMng : public ISceneChanger, Task{

private:
	BaseScene* mScene;	//�V�[���Ǘ��ϐ�
	eScene mNextScene;	//���̃V�[���Ǘ��ϐ�

public:
	SceneMng();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	//���� nextScene �ɃV�[����ύX����
	void ChangeScene(eScene NextScene) override;
};