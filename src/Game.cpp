/* Game.cpp */

#include "DxLib.h"
#include "Game.h"
#include "Keyboard.h"
#include "Menu.h"
#include "SceneMng.h"
#include "Sound.h"
#include "Graphic.h"
#include "Stage.h"
#include "BulletMng.h"
#include "Sound.h"
#include "PlayerMng.h"
#include "ItemMng.h"
#include "Graphic.h"


static class Stage      stage;
static class PlayerMng  playermng;
static class ItemMng    itemmng;
static class Graphic    status;
static class BulletMng  bulletmng;
static class Sound_Func soundf;


static bool f_pause;
static bool f_finishmusic;
static int  h_gameset[2];


Game::Game(ISceneChanger* changer):BaseScene(changer){}


inline void Game::Initialize()
{
	f_pause = false;
	f_finishmusic = false;

	stage.Initialize();
	playermng.Initialize();
	bulletmng.Initialize();
	itemmng.Initialize();
	status.Initialize();
}


inline void Game::InitGraphs()
{
	h_gameset[0] = LoadGraph("GRAPH/GAME/1pwin.png");
	h_gameset[1] = LoadGraph("GRAPH/GAME/2pwin.png");

	stage.InitGraph();
	playermng.InitGraph();
	bulletmng.InitGraphs();
	itemmng.InitGraph();
	status.InitGraph();
}


inline void Game::Finalize()
{
	stage.Finalize();
	playermng.Finalize();
	bulletmng.Finalize();
	itemmng.Finalize();
	status.Finalize();
}


void Game::Update()
{
	// ƒ^ƒCƒgƒ‹ƒw
	if (Keyboard_Get(KEY_INPUT_P) == 1)
	{
		soundf.Stop_Music();
		mSceneChanger->ChangeScene(eScene_Menu);
	}

	// PAUSE
	if (Keyboard_Get(KEY_INPUT_F1) == 1)
		f_pause = !f_pause;

	// GAME SET
	if (FinishGame() == true)
	{
		if (f_finishmusic == false)
		{
			soundf.Play_Music(11);
			f_finishmusic = true;
		}
	}

	if (f_pause == true || FinishGame() == true) return;
	stage.Update();
	playermng.Update();
	bulletmng.Update();
	itemmng.Update();
}


void Game::Draw()
{
	if (FinishGame() == true)
	{
		if (ShowWinnerPlayer() == First)
			DrawGraph(0, 0, h_gameset[First], false);
		if (ShowWinnerPlayer() == Second)
			DrawGraph(0, 0, h_gameset[Second], false);

		return;
	}

	stage.Draw();
	itemmng.Draw();
	bulletmng.Draw();
	playermng.Draw();
	status.Draw();
}