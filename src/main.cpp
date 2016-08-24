/*====================================================================


	 @S.Q.U.A.R.E TYPE 2@ 
	
								@@@@by –î•—I(@onedannitori)

======================================================================*/


#include "DxLib.h"
#include "SceneMng.h"
#include "Menu.h"
#include "Game.h"
#include "Sound.h"
#include "Config.h"
#include "Graphic.h"
#include "Keyboard.h"


static int  FrameStartTime; // 60fpsŒÅ’èê—p
static bool ScSizeFrag;     // ‰æ–Êƒ‚[ƒh•ÏX—p
static int h_load;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib_Init();

	SetMainWindowText("‚r‚p‚t‚`‚q‚d¡‚s‚x‚o‚d2");

	// VGA‚Å‚·‚Í‚¢B640 * 480
	SetGraphMode(640, 480, 32), SetDrawScreen(DX_SCREEN_BACK), ChangeWindowMode(FALSE);

	h_load = LoadGraph("GRAPH/nowloading.png");
	
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE); // ©‰æ–Êƒ‚[ƒh‚ğ•ÏX‚µ‚Ä‚àƒƒ‚ƒŠ‚ğƒŠƒZƒbƒg‚µ‚È‚¢iˆÀ‘S«•s–¾j
	
	SetWaitVSyncFlag(FALSE); // ‚’¼“¯ŠúM†‚ğ‘Ò‚½‚È‚¢
	FrameStartTime = GetNowCount(); // Œ»İ‚ÌƒJƒEƒ“ƒg‚ÉƒZƒbƒg
	ScSizeFrag = FALSE; // ‰Šú‚Íwindowmode

	SceneMng   sceneMng;
	Sound_Func soundf;

	// ƒ[ƒh‰æ–Ê
	ClearDrawScreen();
	DrawRotaGraph(320, 240, 4.0, 0.0, h_load, false, false);
	ScreenFlip();

	//  LOADING
	soundf.Initialize();
	sceneMng.Initialize();
	DeleteGraph(h_load);


//MAIN PROGRAM|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


	while (Menu_Quit() == 0 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		ClsDrawScreen();
		while (GetNowCount() - FrameStartTime < 1000 / 60){} // 1/60 •b‚Ü‚Å‘Ò‚Â
		FrameStartTime = GetNowCount(); // Œ»İ‚ÌƒJƒEƒ“ƒg‚ğ•Û‘¶

		Keyboard_Update(); // ƒL[“ü—Íæ“¾

		if (Keyboard_Get(ESC) == 1)	Menu_Exit(); // [Esc]‚Å‹^—‹­§I—¹
		if (Keyboard_Get(KEY_INPUT_F5) == 1)	ChangeWindowMode(ScSizeFrag = !ScSizeFrag); // ‰æ–Êƒ‚[ƒh•ÏX

		sceneMng.Update();
		sceneMng.Draw();
	}


	DxLib_End();

	return 0;
}