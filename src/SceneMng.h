#pragma once

#include "ISceneChanger.h"
#include "BaseScene.h"

class SceneMng : public ISceneChanger, Task{

private:
	BaseScene* mScene;	//シーン管理変数
	eScene mNextScene;	//次のシーン管理変数

public:
	SceneMng();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	//引数 nextScene にシーンを変更する
	void ChangeScene(eScene NextScene) override;
};