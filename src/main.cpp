/*====================================================================


	���@S.Q.U.A.R.E TYPE 2�@��
	
								�@�@�@�@by ��I(@onedannitori)

======================================================================*/


#include "DxLib.h"
#include "SceneMng.h"
#include "Menu.h"
#include "Game.h"
#include "Sound.h"
#include "Config.h"
#include "Graphic.h"
#include "Keyboard.h"


static int  FrameStartTime; // 60fps�Œ��p
static bool ScSizeFrag;     // ��ʃ��[�h�ύX�p
static int h_load;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib_Init();

	SetMainWindowText("�r�p�t�`�q�d���s�x�o�d2");

	// VGA�ł��͂��B640 * 480
	SetGraphMode(640, 480, 32), SetDrawScreen(DX_SCREEN_BACK), ChangeWindowMode(FALSE);

	h_load = LoadGraph("GRAPH/nowloading.png");
	
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE); // ����ʃ��[�h��ύX���Ă������������Z�b�g���Ȃ��i���S���s���j
	
	SetWaitVSyncFlag(FALSE); // ���������M����҂��Ȃ�
	FrameStartTime = GetNowCount(); // ���݂̃J�E���g�ɃZ�b�g
	ScSizeFrag = FALSE; // ������windowmode

	SceneMng   sceneMng;
	Sound_Func soundf;

	// ���[�h���
	ClearDrawScreen();
	DrawRotaGraph(320, 240, 4.0, 0.0, h_load, false, false);
	ScreenFlip();

	//  LOADING
	soundf.Initialize();
	sceneMng.Initialize();
	DeleteGraph(h_load);


//MAIN PROGRAM�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|


	while (Menu_Quit() == 0 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		ClsDrawScreen();
		while (GetNowCount() - FrameStartTime < 1000 / 60){} // 1/60 �b�܂ő҂�
		FrameStartTime = GetNowCount(); // ���݂̃J�E���g��ۑ�

		Keyboard_Update(); // �L�[���͎擾

		if (Keyboard_Get(ESC) == 1)	Menu_Exit(); // [Esc]�ŋ^�������I��
		if (Keyboard_Get(KEY_INPUT_F5) == 1)	ChangeWindowMode(ScSizeFrag = !ScSizeFrag); // ��ʃ��[�h�ύX

		sceneMng.Update();
		sceneMng.Draw();
	}


	DxLib_End();

	return 0;
}