/*====================================================================


	□　S.Q.U.A.R.E TYPE 2　□
	
								　　　　by 矢風！(@onedannitori)

======================================================================*/


#include "DxLib.h"
#include "SceneMng.h"
#include "Menu.h"
#include "Game.h"
#include "Sound.h"
#include "Config.h"
#include "Graphic.h"
#include "Keyboard.h"


static int  FrameStartTime; // 60fps固定専用
static bool ScSizeFrag;     // 画面モード変更用
static int h_load;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib_Init();

	SetMainWindowText("ＳＱＵＡＲＥ■ＴＹＰＥ2");

	// VGAですはい。640 * 480
	SetGraphMode(640, 480, 32), SetDrawScreen(DX_SCREEN_BACK), ChangeWindowMode(FALSE);

	h_load = LoadGraph("GRAPH/nowloading.png");
	
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE); // ←画面モードを変更してもメモリをリセットしない（安全性不明）
	
	SetWaitVSyncFlag(FALSE); // 垂直同期信号を待たない
	FrameStartTime = GetNowCount(); // 現在のカウントにセット
	ScSizeFrag = FALSE; // 初期はwindowmode

	SceneMng   sceneMng;
	Sound_Func soundf;

	// ロード画面
	ClearDrawScreen();
	DrawRotaGraph(320, 240, 4.0, 0.0, h_load, false, false);
	ScreenFlip();

	//  LOADING
	soundf.Initialize();
	sceneMng.Initialize();
	DeleteGraph(h_load);


//MAIN PROGRAM−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−


	while (Menu_Quit() == 0 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		ClsDrawScreen();
		while (GetNowCount() - FrameStartTime < 1000 / 60){} // 1/60 秒まで待つ
		FrameStartTime = GetNowCount(); // 現在のカウントを保存

		Keyboard_Update(); // キー入力取得

		if (Keyboard_Get(ESC) == 1)	Menu_Exit(); // [Esc]で疑似強制終了
		if (Keyboard_Get(KEY_INPUT_F5) == 1)	ChangeWindowMode(ScSizeFrag = !ScSizeFrag); // 画面モード変更

		sceneMng.Update();
		sceneMng.Draw();
	}


	DxLib_End();

	return 0;
}