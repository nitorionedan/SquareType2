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


//������
void SceneMng::Initialize()
{
	mScene->Initialize();
	mScene->InitGraphs();
}

//�I��
void SceneMng::Finalize()
{
		mScene -> Finalize();
}

//�X�V
void SceneMng::Update()
{
	if (mNextScene != eScene_None) //���̃V�[�����Z�b�g����Ă�����
	{
		mScene->Finalize();
		delete mScene;
		switch (mNextScene)
		{
		case eScene_Menu: //���̃V�[���Ȃ�
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
		mNextScene = eScene_None; //���̃V�[�����N���A
		mScene->Initialize();
		mScene->InitGraphs();
	}

	mScene->Update(); //�V�[���̍X�V
}

//�`��
void SceneMng::Draw()
{
	mScene->Draw();
}

//����
void SceneMng::ChangeScene(eScene NextScene)
{
	mNextScene = NextScene;	//���̃V�[�����Z�b�g����
}