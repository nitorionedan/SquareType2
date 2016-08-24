#include "DxLib.h"
#include "SceneMng.h"
#include "Game.h"
#include "Menu.h"
#include "Config.h"
#include "Sound.h"
#include "Stage.h"
#include "StageSelect.h"
#include "ShipSelect.h"
#include "Result.h"


static class Sound_Func soundf;


SceneMng::SceneMng():mNextScene(eScene_None)
{
	mScene = (BaseScene*) new Menu(this);
}


//初期化
void SceneMng::Initialize()
{
	mScene->Initialize();
	mScene->InitGraphs();
}

//終了
void SceneMng::Finalize()
{
		mScene -> Finalize();
}

//更新
void SceneMng::Update()
{
	if (mNextScene != eScene_None) //次のシーンがセットされていたら
	{
		mScene->Finalize();
		delete mScene;
		switch (mNextScene)
		{
		case eScene_Menu: //次のシーンなら
			mScene = (BaseScene*) new Menu(this);
			break;
		case eScene_Game:
			mScene = (BaseScene*) new Game(this);
			break;
		case eScene_Config:
			mScene = (BaseScene*) new Config(this);
			break;
		case eScene_StageSelect:
			mScene = (BaseScene*) new StageSelect(this);
			break;
		case eScene_ShipSelect:
			mScene = (BaseScene*) new ShipSelect(this);
			break;
		case eScene_Result:
//			mScene = (BaseScene*) new Result(this);
			break;
		}
		mNextScene = eScene_None; //次のシーンをクリア
		mScene->Initialize();
		mScene->InitGraphs();
	}

	mScene->Update(); //シーンの更新
}

//描画
void SceneMng::Draw()
{
	mScene->Draw();
}

//引数
void SceneMng::ChangeScene(eScene NextScene)
{
	mNextScene = NextScene;	//次のシーンをセットする
}