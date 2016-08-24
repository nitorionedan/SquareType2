#include "DxLib.h"
#include "Config.h"
#include "Sound.h"
#include "Keyboard.h"
#include "SceneMng.h"
#include "Menu.h"
#include "Sound.h"
#include "PlayerMng.h"
#include "Result.h"


static class Sound_Func soundf;

static int h_gameset[2];


Result::Result(ISceneChanger* changer) :BaseScene(changer)
{}


void Result::Initialize()
{
	soundf.Play_Music(11);
}


void Result::InitGraphs()
{
	h_gameset[0] = LoadGraph("GRAPH/GAME/1pwin.png");
	h_gameset[1] = LoadGraph("GRAPH/GAME/2pwin.png");
}


void Result::Finalize()
{
	for (int i = 0; i < 2; i++)	DeleteGraph(h_gameset[i]);
}


void Result::Update()
{
	if (Keyboard_Get(KEY_INPUT_P) == 1)
	{
		soundf.Stop_Music();
		mSceneChanger->ChangeScene(eScene_Menu);
	}
}


void Result::Draw()
{
	if (ShowWinnerPlayer() == First)
		DrawGraph(0, 0, h_gameset[First], false);
	if (ShowWinnerPlayer() == Second)
		DrawGraph(0, 0, h_gameset[Second], false);
}