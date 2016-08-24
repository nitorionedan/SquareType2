#include "DxLib.h"
#include "ShipSelect.h"
#include "Sound.h"
#include "Keyboard.h"
#include "PlayerMng.h"


static class ShipSelect_Graph shipselect;
static class ShipPtr shipptr;
static class Sound_Func soundf;
static class Sign sign;


static int MasX[2][4], MasY[2][3];
static int h_ship[4];
static int h_ready;
static bool f_ready;
static enum PlayerType TempPlayerType[2];

static const int MasSize = 80;


ShipSelect::ShipSelect(ISceneChanger* changer) :BaseScene(changer){}


void ShipSelect::Initialize()
{
	// 40 281
	for (int i = 0; i < 4; i++)
		MasX[0][i] = 40 + (i * MasSize);
	for (int i = 0; i < 4; i++)
		MasY[0][i] = 280 + (i * MasSize);

	for (int i = 0; i < 4; i++)
		MasX[1][i] = 360 + (i * MasSize);
	for (int i = 0; i < 4; i++)
		MasY[1][i] = 280 + (i * MasSize);

	shipselect.x = 320;
	shipselect.y = 240;


	for (int i = 0; i < 2; i++)
	{
		shipptr.MasX[i] = 0;
		shipptr.MasY[i] = 0;
		shipptr.f_IsSelect[i] = false;
	}

	f_ready = false;

	soundf.Play_Music(13);
}


void ShipSelect::InitGraphs()
{
	shipselect.h = LoadGraph("GRAPH/GAME/charaselect.png");
	shipptr.h[0] = LoadGraph("GRAPH/GAME/ch_selectptr00.png");
	shipptr.h[1] = LoadGraph("GRAPH/GAME/ch_selectptr01.png");

	h_ship[0] = LoadGraph("GRAPH/GAME/PLAYER/red00.png");
	h_ship[1] = LoadGraph("GRAPH/GAME/PLAYER/blue00.png");
	h_ship[2] = LoadGraph("GRAPH/GAME/PLAYER/purple00.png");
	h_ship[3] = LoadGraph("GRAPH/GAME/PLAYER/gomi.png");

	for (int i = 0; i < 2; i++)
		sign.h[i] = LoadGraph("GRAPH/GAME/kettei.png");

	h_ready = LoadGraph("GRAPH/GAME/areyou.png");
}


void ShipSelect::Finalize()
{
	DeleteGraph(shipselect.h);
	for (int i = 0; i < 2; i++)	DeleteGraph(shipptr.h[i]);
	for (int i = 0; i < 4; i++)	DeleteGraph(h_ship[i]);
	for (int i = 0; i < 2; i++)	DeleteGraph(sign.h[i]);
	DeleteGraph(h_ready);
}


void ShipSelect::Update()
{
	if (f_ready)
	{
		// ŠJŽn‚µ‚Ü‚·‚©H
		// SPACEKEY
		if (Keyboard_Get(SPACE) == 1)
		{
			soundf.SE_NORMAL(2);
			soundf.Stop_Music();
			mSceneChanger->ChangeScene(eScene_StageSelect);
		}
	}

	if (f_ready)	return;

	if (Keyboard_Get(RIGHT) == 1 || Keyboard_Get(LEFT) == 1)	soundf.SE(1);
	if (Keyboard_Get(UP) == 1 || Keyboard_Get(DOWN) == 1)		soundf.SE(1);
	if (Keyboard_Get(KA) == 1 || Keyboard_Get(KD) == 1)			soundf.SE(1);
	if (Keyboard_Get(KS) == 1 || Keyboard_Get(KW) == 1)			soundf.SE(1);

	// 1P
	if (shipptr.f_IsSelect[0] == false)
	{
		if (Keyboard_Get(KD) == 1)
			shipptr.MasX[0] = (shipptr.MasX[0] + 1) % 4;
		if (Keyboard_Get(KA) == 1)
			shipptr.MasX[0] = (shipptr.MasX[0] + 3) % 4;
		if (Keyboard_Get(KS) == 1)
			shipptr.MasY[0] = (shipptr.MasY[0] + 1) % 3;
		if (Keyboard_Get(KW) == 1)
			shipptr.MasY[0] = (shipptr.MasY[0] + 2) % 3;
	}

	// 2P
	if (shipptr.f_IsSelect[1] == false)
	{
		if (Keyboard_Get(RIGHT) == 1)
			shipptr.MasX[1] = (shipptr.MasX[1] + 1) % 4;
		if (Keyboard_Get(LEFT) == 1)
			shipptr.MasX[1] = (shipptr.MasX[1] + 3) % 4;
		if (Keyboard_Get(DOWN) == 1)
			shipptr.MasY[1] = (shipptr.MasY[1] + 1) % 3;
		if (Keyboard_Get(UP) == 1)
			shipptr.MasY[1] = (shipptr.MasY[1] + 2) % 3;
	}

	// Move
	for (int i = 0; i < 4; i++)
		if (shipptr.MasX[0] == i)	shipptr.x[0] = MasX[0][i];
	for (int i = 0; i < 3; i++)
		if (shipptr.MasY[0] == i)	shipptr.y[0] = MasY[0][i];

	for (int i = 0; i < 4; i++)
		if (shipptr.MasX[1] == i)	shipptr.x[1] = MasX[1][i];
	for (int i = 0; i < 3; i++)
		if (shipptr.MasY[1] == i)	shipptr.y[1] = MasY[1][i];

	// 1P ‚ªŒˆ’è‚µ‚½‚Æ‚«
	if (shipptr.f_IsSelect[0] == false && Keyboard_Get(SPACE) == 1)
	{
		soundf.SE(2);

		if (shipptr.MasX[0] == 0 && shipptr.MasY[0] == 0)
			TempPlayerType[First] = Normal;
	
		if (shipptr.MasX[0] == 1 && shipptr.MasY[0] == 0)
			TempPlayerType[First] = Blue;
		
		if (shipptr.MasX[0] == 2 && shipptr.MasY[0] == 1)
			TempPlayerType[First] = Purple;
		
		if (shipptr.MasX[0] == 3 && shipptr.MasY[0] == 2)
			TempPlayerType[First] = Black;

		shipptr.f_IsSelect[0] = true;
	}

	// 2P‚ªŒˆ’è‚µ‚½‚Æ‚«
	if (shipptr.f_IsSelect[1] == false && Keyboard_Get(ENTER) == 1)
	{
		soundf.SE(2);

		if (shipptr.MasX[1] == 0 && shipptr.MasY[1] == 0)
			TempPlayerType[Second] = Normal;

		if (shipptr.MasX[1] == 1 && shipptr.MasY[1] == 0)
			TempPlayerType[Second] = Blue;
		
		if (shipptr.MasX[1] == 2 && shipptr.MasY[1] == 1)
			TempPlayerType[Second] = Purple;
		
		if (shipptr.MasX[1] == 3 && shipptr.MasY[1] == 2)
			TempPlayerType[Second] = Black;

		shipptr.f_IsSelect[1] = true;
	}

	// Return
	if (Keyboard_Get(KEY_INPUT_X) == 1)
	{
		soundf.Stop_Music();
		mSceneChanger->ChangeScene(eScene_Menu);
	}

	if (shipptr.f_IsSelect[First] == TRUE && shipptr.f_IsSelect[Second] == TRUE) f_ready = true;
}


void ShipSelect::Draw()
{
	DrawRotaGraph(shipselect.x, shipselect.y, 2.0, 0.0, shipselect.h, TRUE, FALSE);

	for (int i = 0; i < 2; i++)
	{
		DrawRotaGraph(MasX[i][0], MasY[i][0], 2.0, 0.0, h_ship[0], TRUE);
		DrawRotaGraph(MasX[i][1], MasY[i][0], 2.0, 0.0, h_ship[1], TRUE);
		DrawRotaGraph(MasX[i][2], MasY[i][1], 2.0, 0.0, h_ship[2], TRUE);
		DrawRotaGraph(MasX[i][3], MasY[i][2], 2.0, 0.0, h_ship[3], TRUE);
	}

	for (int i = 0; i < 2; i++)
	{
		if (shipptr.f_IsSelect[i] == false)
			DrawRotaGraph(shipptr.x[i], shipptr.y[i], 2.0, 0.0, shipptr.h[i], TRUE);
		else
		{
			DrawRotaGraph(shipptr.x[i], shipptr.y[i], 2.0, 0.0, sign.h[i], TRUE);
		}
	}

	if (shipptr.f_IsSelect[First] == TRUE)
	{
		if (TempPlayerType[First] == Normal)
			DrawRotaGraph(125, 115, 2.0, 0.0, h_ship[0], TRUE);
		if (TempPlayerType[First] == Blue)
			DrawRotaGraph(125, 115, 2.0, 0.0, h_ship[1], TRUE);
		if (TempPlayerType[First] == Purple)
			DrawRotaGraph(125, 115, 2.0, 0.0, h_ship[2], TRUE);
		if (TempPlayerType[First] == Black)
			DrawRotaGraph(125, 115, 2.0, 0.0, h_ship[3], TRUE);
	}
	if (shipptr.f_IsSelect[Second] == TRUE)
	{
		if (TempPlayerType[Second] == Normal)
			DrawRotaGraph(515, 115, 2.0, 0.0, h_ship[0], TRUE);
		if (TempPlayerType[Second] == Blue)
			DrawRotaGraph(515, 115, 2.0, 0.0, h_ship[1], TRUE);
		if (TempPlayerType[Second] == Purple)
			DrawRotaGraph(515, 115, 2.0, 0.0, h_ship[2], TRUE);
		if (TempPlayerType[Second] == Black)
			DrawRotaGraph(515, 115, 2.0, 0.0, h_ship[3], TRUE);
	}

	if (f_ready)
		DrawRotaGraph(320, 240, 2.0, 0.0, h_ready, false);
}


enum PlayerType GetPlayerType(const enum PlayerNum num)
{
	return TempPlayerType[num];
}