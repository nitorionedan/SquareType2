/* Menu.cpp */

#include "Menu.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "Keyboard.h"
#include "Sound.h"
#include "Graphic.h"
#include "Stage.h"
#include "StageDate.h"
#include "Sound.h"


static class Menu_Graph title, yameru;
static class Menu_Select menu;
static class Stage stage;
static class Sound_Func soundf;

static int  quit = 0;
static int* pquit = &quit;

static int i; // いらん
static int SelectUp, SelectMiddle, SelectUnder;
static int c_Menu; // menu move counter
static bool f_MenuIsMove;
static enum KeyComand keycmd;
static int h_att; // 操作説明（ホントはいれたくない...デザイン的に...）


static const int c_MenuTime = 20;


Menu::Menu(ISceneChanger* changer):BaseScene(changer)
{}

void Menu::InitGraphs()
{
	title.h = LoadGraph("GRAPH/TITLE/title00.png");
	yameru.h = LoadGraph("GRAPH/TITLE/quit2.png");

	menu.h[0] = LoadGraph("GRAPH/TITLE/tutorial.png");
	menu.h[1] = LoadGraph("GRAPH/TITLE/2playmode.png");
	menu.h[2] = LoadGraph("GRAPH/TITLE/freemode.png");
	menu.h[3] = LoadGraph("GRAPH/TITLE/option.png");
	h_att     = LoadGraph("GRAPH/TITLE/attention.png");
}


void Menu::Initialize()
{
	c_Menu = 0;
	f_MenuIsMove = false;
	title.x = title.y = 0;
	yameru.x = 20;
	yameru.y = 20;
	for (i = 0; i < 4; i++)	menu.x[i] = 320;
	menu.y[3] = 280.0;
	menu.y[0] = 320.0;
	menu.y[1] = 360.0;
	menu.y[2] = -100.0;
	menu.menuNum = 0;
	
	for (i = 0; i < 4; i++)	menu.exrate[i] = 2.0;
	
	menu.kind[3] = Up;
	menu.kind[0] = Middle;
	menu.kind[1] = Under;
	menu.kind[2] = None;
	menu.exrate[3] = 1.5;
	menu.exrate[0] = 2.5;
	menu.exrate[1] = 1.5;
	menu.exrate[2] = 1.5;

	soundf.Play_Music(1);
}


void Menu::Finalize()
{
	for (i = 0; i < 4; i++)	DeleteGraph(menu.h[i]);
	DeleteGraph(title.h);
	DeleteGraph(yameru.h);
}


void Menu::Update()
{
	if (f_MenuIsMove)	c_Menu++;
	if (c_Menu > c_MenuTime)
	{
		c_Menu = 0;
		f_MenuIsMove = false;
	}

	if (c_Menu == 0)
	{
		if (Keyboard_Get(UP) == 1 || Keyboard_Get(DOWN) == 1)
		{
			f_MenuIsMove = true;
			soundf.SE(0);
		}
		if (Keyboard_Get(UP) == 1)	keycmd = UpArrow;
		if (Keyboard_Get(DOWN) == 1) keycmd = DownArrow;

		if (Keyboard_Get(UP) == 1)		menu.menuNum = (menu.menuNum + 3) % 4;
		if (Keyboard_Get(DOWN) == 1)	menu.menuNum = (menu.menuNum + 1) % 4;
	}

	// メニューの動き
	menu.Move(keycmd);

	if (f_MenuIsMove) return;

	if (Keyboard_Get(UP) == 0 && Keyboard_Get(DOWN) == 0) keycmd = NoneCmd;


	if (Keyboard_Get(KEY_INPUT_Z) == 1 || Keyboard_Get(KEY_INPUT_SPACE) == 1 || Keyboard_Get(KEY_INPUT_RETURN) == 1)
	{
		soundf.SE_NORMAL(2);

		if (menu.menuNum == 0)
		{
			soundf.Stop_Music();
			mSceneChanger->ChangeScene(eScene_Game);
			GetNowStageType(SampleStage); // Debug : 現在のステージをサンプルステージにする
			GetStageNum(Example, 0, ShowStageType());
		}

		// StageSelectへ移行
		if (menu.menuNum == 1)
		{
			soundf.Stop_Music();
			mSceneChanger->ChangeScene(eScene_ShipSelect);
		}

		if (menu.menuNum == 2)
		{
//			soundf.Stop_Music();
//			mSceneChanger->ChangeScene(eScene_StageSelect);
		}

		if (menu.menuNum == 3)
		{
//			soundf.Stop_Music();
//			mSceneChanger->ChangeScene(eScene_Config);
		}
	}
}


void Menu::Draw()
{
	DrawGraph(title.x, title.y, title.h, TRUE);
	DrawGraph(yameru.x, yameru.y, yameru.h, TRUE);

	for(i = 0; i < 4; i++)	DrawRotaGraph(menu.x[i], menu.y[i], menu.exrate[i], 0.0, menu.h[i], TRUE, FALSE);
	
	DrawRotaGraph(360, -80, 1.0, 0.0, h_att, TRUE);
}

//　main to quit = 1
int Menu_Quit()
{
	return *pquit;
}

// Quit...
void Menu_Exit()
{
	*pquit = 1;
}


void Menu_Select::Move(const KeyComand cmd)
{
	if (cmd == NoneCmd) return;

	if (cmd == DownArrow)
	{
		for (i = 0; i < 4; i++)
		{
			if (!f_MenuIsMove) break;

			if (menu.kind[i] == Up)
			{
				menu.x[i] = -1000;
				if (c_Menu == c_MenuTime)	menu.kind[i] = None;
				continue;
			}
			if (menu.kind[i] == Middle)
			{
				menu.y[i] -= 40.0 / c_MenuTime;
				menu.exrate[i] -= 1.0 / c_MenuTime;
				if (c_Menu == c_MenuTime)	menu.kind[i] = Up;
				continue;
			}
			if (menu.kind[i] == Under)
			{
				menu.y[i] -= 40.0 / c_MenuTime;
				menu.exrate[i] += 1.0 / c_MenuTime;

				if (c_Menu == c_MenuTime)	menu.kind[i] = Middle;
				continue;
			}
			if (menu.kind[i] == None)
			{
				if (c_Menu == c_MenuTime)
				{
					menu.exrate[i] = 1.5;
					menu.y[i] = 360.0;
					menu.x[i] = 320;
					menu.kind[i] = Under;
				}
			}
		}
	}

	if (cmd == UpArrow)
	{
		for (i = 0; i < 4; i++)
		{
			if (!f_MenuIsMove) break;

			if (menu.kind[i] == Up)
			{
				menu.y[i] += 40.0 / c_MenuTime;
				menu.exrate[i] += 1.0 / c_MenuTime;
				if (c_Menu == c_MenuTime)	menu.kind[i] = Middle;
				continue;
			}
			if (menu.kind[i] == Middle)
			{
				menu.y[i] += 40.0 / c_MenuTime;
				menu.exrate[i] -= 1.0 / c_MenuTime;
				if (c_Menu == c_MenuTime)	menu.kind[i] = Under;
				continue;
			}
			if (menu.kind[i] == Under)
			{
				menu.x[i] = -1000;
				if (c_Menu == c_MenuTime)	menu.kind[i] = None;
				continue;
			}
			if (menu.kind[i] == None)
			{
				if (c_Menu == c_MenuTime)
				{
					menu.y[i] = 280.0;
					menu.x[i] = 320;
					menu.exrate[i] = 1.5;
					menu.kind[i] = Up;
				}
			}
		}
	}
}