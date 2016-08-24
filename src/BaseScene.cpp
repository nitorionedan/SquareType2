#include "BaseScene.h"
#include "DxLib.h"
#include "Sound.h"


BaseScene::BaseScene(ISceneChanger* changer)
{
	mSceneChanger = changer;
}

void BaseScene::Finalize(){}

void BaseScene::Draw(){}