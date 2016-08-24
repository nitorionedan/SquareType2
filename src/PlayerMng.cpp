#pragma warning(disable:4786)
#pragma warning(disable:4244)

#include <cmath>
#include <vector>

#include "DxLib.h"
#include "PlayerMng.h"
#include "Keyboard.h"
#include "Menu.h"
#include "SceneMng.h"
#include "Sound.h"
#include "Graphic.h"
#include "Stage.h"
#include "Sound.h"
#include "ItemMng.h"
#include "ShipSelect.h"
#include "ISceneChanger.h"
#include "BaseScene.h"


using namespace std;


static class Player			player[2], players;
static class PlayerFunction plyfunc;
static class Block			block;
static class Pointer		pointer[2]; // 1P か 2P か知らせる GUI
static class Graph_Date		graph;
static class Sound_Func		sound;
static class HPmemory		hpmem[2]; // HP memory graph
static class PowerMemory	powmem[2]; // Power memory graph
static class StatusBar		statusbar[2];
static class Zanki			zanki[2];
static class ItemFunction	itemfunc;
static class Meter			meterSP[2], meterAC[2], meterRT[2]; // Status meters.
static class P_ItemEffect   itemef;

static int    c_test; // アニメ用カウンター

static double iatk[2]; // plus points
static double imaxhp[2];
static double imaxspd[2];
static double irota[2];
static double iaccel[2];
static double SwapposiX, SwapposiY; // アイテム「どくろ」用変数
static int    numsp[2], numac[2], numrt[2]; // Status memori
static int    h_boom;
static int    c_finish;
static bool   f_IsFinish;

static const double Fric	 = 0.1;
static const double Pai		 = 3.14159265359;
static const double MaxSpeed = 3.0; // デバッグ用
static const double Accel    = 0.05;
static const double ScTop    = 50.0;
static const double ScBottom = 470.0;
static const double ScLeft   = 6.0;
static const double ScRight  = 630.0;
static const double RangeX   = 8.0, RangeY = 8.0; // Hit range.
static const double hpup     = 1.0, atkup = 1.0, spup = 0.15, rtup = 0.005, acup = 0.01; // up memori.
static const int PointerTime       = 100;
static const int PointerFirstFrame = PointerTime - 1;
static const int PointerNum        = ( sizeof(pointer[0].h) / sizeof(pointer[0].h[0]) ) / 2;


void PlayerMng::Initialize()
{
	c_test = 0;
	c_finish = 0;
	f_IsFinish = false;

	// なぜかここにおくことに...
	// red
	graph.vh_red.push_back(LoadGraph("GRAPH/GAME/PLAYER/red00.png"));
	graph.vh_red.push_back(LoadGraph("GRAPH/GAME/PLAYER/red00_hit00.png"));
	graph.vh_red.push_back(LoadGraph("GRAPH/GAME/PLAYER/red00_hit01.png"));

	// blue
	graph.vh_blue.push_back(LoadGraph("GRAPH/GAME/PLAYER/blue00.png"));
	graph.vh_blue.push_back(LoadGraph("GRAPH/GAME/PLAYER/blue00_hit00.png"));
	graph.vh_blue.push_back(LoadGraph("GRAPH/GAME/PLAYER/blue00_hit01.png"));
	
	// purple
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple00.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple01.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple02.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple03.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple00_hit00.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple00_hit01.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple01_hit00.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple01_hit01.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple02_hit00.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple02_hit01.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple03_hit00.png"));
	graph.vh_purple.push_back(LoadGraph("GRAPH/GAME/PLAYER/purple03_hit01.png"));

	graph.vh_black.push_back(LoadGraph("GRAPH/GAME/PLAYER/gomi.png"));

	graph.vh_statusbar.push_back(LoadGraph("GRAPH/GAME/redbar01.png"));
	graph.vh_statusbar.push_back(LoadGraph("GRAPH/GAME/bluebar.png"));
	graph.vh_statusbar.push_back(LoadGraph("GRAPH/GAME/purplebar.png"));
	graph.vh_statusbar.push_back(LoadGraph("GRAPH/GAME/blackbar.png"));

	graph.vh_zanki.push_back(LoadGraph("GRAPH/GAME/zanki03.png"));

	// Get random (:Characters position)
	SRand(GetNowCount());

	// Player
	player[First].VectorX = GetRand(575) + 30.0;
	player[First].VectorY = GetRand(365) + 80.0; //   …名づけを間違えたので余裕が有ったら改善

	player[Second].VectorX = GetRand(575) + 30.0;
	player[Second].VectorY = GetRand(365) + 80.0;

	for (int i = 0; i < 2; i++)
	{
		player[i].zanki = 2;
		player[i].angle = GetRand(100);
		player[i].speed = 0.0;
		player[i].f_damage = false;

		player[i].l = player[i].VectorX - RangeX;
		player[i].r = player[i].VectorX + RangeX;
		player[i].t = player[i].VectorY - RangeY;
		player[i].b = player[i].VectorY + RangeY;
	}

	plyfunc.Initialize_Type(First,  GetPlayerType(First));
	plyfunc.Initialize_Type(Second, GetPlayerType(Second));


	for (int i = 0; i < 2; i++)
	{
		// ポインター(機体の上のタグ)
		pointer[i].f_posi = true;
		pointer[i].c_h = 0;

		// ステータスボード
		statusbar[i].x = i * 320;
		statusbar[i].y = 0;
		if (player[i].type == Normal)
		{
			statusbar[i].h      = graph.vh_statusbar[0];
			statusbar[i].h_icon = graph.vh_red[0];
		}
		if (player[i].type == Blue)
		{
			statusbar[i].h = graph.vh_statusbar[1];
			statusbar[i].h_icon = graph.vh_blue[0];
		}
		if (player[i].type == Purple)
		{
			statusbar[i].h = graph.vh_statusbar[2];
			statusbar[i].h_icon = graph.vh_purple[3];
		}
		if (player[i].type == Black)
		{
			statusbar[i].h = graph.vh_statusbar[3];
			statusbar[i].h_icon = graph.vh_black[0];
		}

		// Flags
		player[i].f_exist = true;
		player[i].f_blockatk = true;

		// アイテムプラス値をリセット ALL ZERO
		iatk[i]    = 0.0;
		imaxhp[i]  = 0.0;
		imaxspd[i] = 0.0;
		irota[i]   = 0.0;
		iaccel[i]  = 0.0;
		numsp[i] = numac[i] = numrt[i] = 0;
	}

	// Status graphs
	hpmem[0].x = 40;
	hpmem[0].y = 32;
	hpmem[1].x = hpmem[0].x + 320;
	hpmem[1].y = hpmem[0].y;
	powmem[0].x = 87;
	powmem[0].y = 16;
	powmem[1].x = powmem[0].x + 320;
	powmem[1].y = powmem[0].y;
}


void PlayerFunction::Initialize_Type(enum PlayerNum playernum, enum PlayerType playertype)
{
	if (playertype == Normal)
	{
		player[playernum].h        = graph.vh_red[0];
		player[playernum].rota     = 0.04;
		player[playernum].Maxspeed = 4.0;
		player[playernum].atk      = 1.0;
		player[playernum].Maxhp    = 20.0;
		player[playernum].hp       = player[playernum].Maxhp;
		player[playernum].Accel    = 0.05;
		player[playernum].Inertia  = 0.1;
		player[playernum].wepon_type = Buster;
	}

	if (playertype == Blue)
	{
		player[playernum].h        = graph.vh_blue[0];
		player[playernum].rota     = 0.04;
		player[playernum].Maxspeed = 4.5;
		player[playernum].atk      = 1.0;
		player[playernum].Maxhp    = 20.0;
		player[playernum].hp       = player[playernum].Maxhp;
		player[playernum].Accel    = 0.05;
		player[playernum].Inertia  = 0.1;
		player[playernum].wepon_type = Buster;
	}

	if (playertype == Purple)
	{
		player[playernum].h		   = graph.vh_purple[3];
		player[playernum].rota     = 0.05;
		player[playernum].Maxspeed = 3.5;
		player[playernum].atk      = 1.0;
		player[playernum].Maxhp	   = 15.0;
		player[playernum].hp	   = player[playernum].Maxhp;
		player[playernum].Accel    = 0.03;
		player[playernum].Inertia  = 0.1;
		player[playernum].wepon_type = Buster;
	}
	
	if (playertype == Speedy)
	{}

	if (playertype == Attacky)
	{}

	if (playertype == Black)
	{
		player[playernum].h = graph.vh_black[0];
		player[playernum].rota = 0.07;
		player[playernum].Maxspeed = 3.0;
		player[playernum].atk = 1.0;
		player[playernum].Maxhp = 20.0;
		player[playernum].hp = player[playernum].Maxhp;
		player[playernum].Accel = 0.07;
		player[playernum].Inertia = 0.1;
		player[playernum].wepon_type = Buster;
	}

	if (playertype == Orange)
	{}

	player[playernum].Fric = player[playernum].Accel / 2.0;
}


void PlayerMng::InitGraph()
{
	LoadDivGraph("GRAPH/GAME/pointers00.png", 8, 2, 4, 11, 17, pointer[First].h);
	LoadDivGraph("GRAPH/GAME/pointers01.png", 8, 2, 4, 12, 17, pointer[Second].h);
	for (int i = 0; i < 2; i++)
	{
		LoadDivGraph("GRAPH/GAME/meter_sp.png", 6, 3, 2, 35, 31, meterSP[i].h);
		LoadDivGraph("GRAPH/GAME/meter_ac.png", 6, 3, 2, 35, 31, meterAC[i].h);
		LoadDivGraph("GRAPH/GAME/meter_rt.png", 6, 3, 2, 35, 31, meterRT[i].h);
		hpmem[i].h[0] = LoadGraph("GRAPH/GAME/hpmemori00.png");
		hpmem[i].h[1] = LoadGraph("GRAPH/GAME/hpmemori01.png");
		powmem[i].h   = LoadGraph("GRAPH/GAME/pmemori00.png");
	}

	h_boom = LoadGraph("GRAPH/GAME/boom.png");
}


void PlayerMng::Finalize()
{
	DeleteGraph(player[First].h);
	DeleteGraph(player[Second].h);
	DeleteGraph(h_boom);
	for (auto itr = graph.vh_red.begin();    itr < graph.vh_red.end();    ++itr)	DeleteGraph(*itr);
	for (auto itr = graph.vh_blue.begin();   itr < graph.vh_blue.end();   ++itr)	DeleteGraph(*itr);
	for (auto itr = graph.vh_purple.begin(); itr < graph.vh_purple.end(); ++itr)	DeleteGraph(*itr);
	for (auto itr = graph.vh_black.begin(); itr < graph.vh_black.end(); ++itr)	DeleteGraph(*itr);
	for (auto itr = graph.vh_statusbar.begin(); itr < graph.vh_statusbar.end(); ++itr)	DeleteGraph(*itr);
	for (auto itr = graph.vh_zanki.begin(); itr < graph.vh_zanki.end(); ++itr)	DeleteGraph(*itr);

	graph.vh_red.clear();
	graph.vh_blue.clear();
	graph.vh_purple.clear();
	graph.vh_statusbar.clear();
	graph.vh_zanki.clear();
	graph.vh_black.clear();
}


void PlayerMng::Update()
{
	for (int i = 0; i < 2; i++)
	{
		if (player[i].f_exist == false)
		{
			c_finish++;
			if (c_finish == 120)	f_IsFinish = true;

			return;
		}
	}

	if (player[0].f_exist == false) return;
	if (player[1].f_exist == false) return;

	plyfunc.Update_Players(First);
	plyfunc.Update_Players(Second);
}


void PlayerFunction::Update_Players(PlayerNum num)
{
	c_test++; // ポインターのアニメ用カウンター

	// pointer
	pointer[num].c_h = c_test % PointerTime * PointerNum;

	// upper
	if (player[num].VectorY > 76) // 画面上部に隠れないように
	{
		if (pointer[num].f_posi == false) pointer[num].f_posi = true;
		pointer[num].x = player[num].VectorX;
		pointer[num].y = player[num].VectorY - 25;
	}

	// under
	else
	{
		if (pointer[num].f_posi == true) pointer[num].f_posi = false;
		pointer[num].x = player[num].VectorX;
		pointer[num].y = player[num].VectorY + 25;
	}


	// move
	player[num].VectorX += cos(player[num].angle) * player[num].speed;
	player[num].VectorY += sin(player[num].angle) * player[num].speed;

	if (num == First)
	{
		// Accel
		if (Keyboard_Get(KW) >= 1)
			if (player[num].speed < player[num].Maxspeed + imaxspd[num])	player[num].speed += player[num].Accel + iaccel[num];
		if (Keyboard_Get(KS) >= 1)
			if (player[num].speed > -(player[num].Maxspeed + imaxspd[num]))	player[num].speed -= player[num].Accel + iaccel[num];

		// 1P Inertia
		if (Keyboard_Get(KS) == 0 && Keyboard_Get(KW) == 0)
		{
			if (player[num].speed > 0.0)
			{
				player[num].speed -= player[num].Fric;
				if (player[num].speed < 0.0)	player[num].speed = 0.0;
			}

			if (player[num].speed < 0.0)
			{
				player[num].speed += player[num].Fric;
				if (player[num].speed > 0.0)	player[num].speed = 0.0;
			}
		}

		// 1P Change Angle
		if (Keyboard_Get(KA) >= 1)	player[num].angle -= (player[num].rota + irota[num]);
		if (Keyboard_Get(KD) >= 1)	player[num].angle += (player[num].rota + irota[num]);
	}

	if (num == Second)
	{
		// Accel
		if (Keyboard_Get(UP) >= 1)	
			if (player[num].speed < player[num].Maxspeed + imaxspd[Second])	player[num].speed += (player[num].Accel + iaccel[num]);
		if (Keyboard_Get(DOWN) >= 1)
			if (player[num].speed > -(player[num].Maxspeed + imaxspd[Second]))	player[num].speed -= (player[num].Accel + iaccel[num]);

		// Inertia
		if (Keyboard_Get(DOWN) == 0 && Keyboard_Get(UP) == 0)
		{
			if (player[num].speed > 0.0)
			{
				player[num].speed -= player[num].Fric;
				if (player[num].speed < 0.0)	player[num].speed = 0.0;
			}

			if (player[num].speed < 0.0)
			{
				player[num].speed += player[num].Fric;
				if (player[num].speed > 0.0)	player[num].speed = 0.0;
			}
		}

		// 2P Change Angle
		if (Keyboard_Get(LEFT) >= 1)	player[num].angle -= player[num].rota + irota[num];
		if (Keyboard_Get(RIGHT) >= 1)	player[num].angle += player[num].rota + irota[num];
	}

	// ブロックにあたったとき
	if (player[num].f_blockatk == true)
	{
		if (block.ShowHitTrigger2(player[num].l, player[num].r, player[num].t, player[num].b, player[num].speed, num) == true){}
	}

	// limit of screen size.
	if (player[num].VectorX < ScLeft)	player[num].VectorX = ScLeft;
	if (player[num].VectorX > ScRight)	player[num].VectorX = ScRight;
	if (player[num].VectorY < ScTop)	player[num].VectorY = ScTop;
	if (player[num].VectorY > ScBottom)	player[num].VectorY = ScBottom;

	// Player's Hit Range
	player[num].l = player[num].VectorX - RangeX;
	player[num].r = player[num].VectorX + RangeX;
	player[num].t = player[num].VectorY - RangeY;
	player[num].b = player[num].VectorY + RangeY;

	// アイテムに触れた時
	itemfunc.CheckHit(player[num].l, player[num].r, player[num].t, player[num].b, num);
}


void PlayerMng::Draw()
{
	SetDrawMode(DX_DRAWMODE_BILINEAR); // ON ジグザグ補正

	for(int i = 0; i < 2; i++) players.Draw(&player[i], &pointer[i], &statusbar[i]);
	
	SetDrawMode(DX_DRAWMODE_NEAREST); // OFF ジグザグ補正

	for (int i = 0; i < 2; i++)
	{
		// Status bar
		DrawGraph(statusbar[i].x, statusbar[i].y, statusbar[i].h, false);
		DrawRotaGraph(statusbar[i].x + 20, statusbar[i].y + 20, 1.0, 0.0, statusbar[i].h_icon, true, false);
		DrawGraph(160 + (i * 320), 0, meterSP[i].h[numsp[i]], TRUE);
		DrawGraph(200 + (i * 320), 0, meterAC[i].h[numac[i]], TRUE);
		DrawGraph(240 + (i * 320), 0, meterRT[i].h[numrt[i]], TRUE);

		// POWER
		for (int j = 0; j < iatk[i]; j++)	DrawGraph(powmem[i].x + (j * 8), powmem[i].y, powmem[i].h, true);

		// HP
		for (int j = 0; j < (player[i].Maxhp + imaxhp[i]); j++)
		{
			DrawGraph(hpmem[i].x + (j * 5), hpmem[i].y-1, hpmem[i].h[1], false);
			for (int k = 0; k < player[i].hp; k++)	DrawGraph(hpmem[i].x + (k * 5), hpmem[i].y - 1, hpmem[i].h[0], false);
		}
	}
}


void PlayerFunction::Draw_Players(Player* player, Pointer* pointer, StatusBar* statusbar)
{
	// Player
	if (player->f_exist == TRUE)
		DrawRotaGraph(player->VectorX, player->VectorY, 1.0, (player->angle + Pai / 2), player->h, true, false);
	if (player->f_exist == FALSE)
		DrawRotaGraph(player->VectorX, player->VectorY, 1.0, 0.0, h_boom, true, false);

	// Pointer△
	for (int j = 0; j < PointerNum; j++)
	{
		// Upper
		if (pointer->f_posi == true)
			if (pointer->c_h >= j * PointerTime && pointer->c_h <= PointerFirstFrame + (j * PointerTime)) DrawRotaGraph(pointer->x, pointer->y, 1.0, 0.0, pointer->h[j * 2], true, false);
		// Under
		if (pointer->f_posi == false)
			if (pointer->c_h >= j * PointerTime && pointer->c_h <= PointerFirstFrame + (j * PointerTime)) DrawRotaGraph(pointer->x, pointer->y, 1.0, 0.0, pointer->h[1 + (j * 2)], true, false);
	}
}


/*
void PlayerFunction::Draw_Players(PlayerNum num)
{
	// Player
	DrawRotaGraph(player[num].VectorX, player[num].VectorY, 1.0, (player[num].angle + Pai / 2), player[num].h, true, false);

	// Pointer
	for (int j = 0; j < PointerNum; j++)
	{
		// Upper
		if (pointer[num].f_posi == true)
			if (pointer[num].c_h >= j * PointerTime && pointer[num].c_h <= PointerFirstFrame + (j * PointerTime)) DrawRotaGraph(pointer[num].x, pointer[num].y, 1.0, 0.0, pointer[num].h[j * 2], true, false);
		// Under
		if (pointer[num].f_posi == false)
			if (pointer[num].c_h >= j * PointerTime && pointer[num].c_h <= PointerFirstFrame + (j * PointerTime)) DrawRotaGraph(pointer[num].x, pointer[num].y, 1.0, 0.0, pointer[num].h[1 + (j * 2)], true, false);
	}

	// Status bar
	DrawGraph(statusbar[num].x, statusbar[num].y, statusbar[num].h, false);
	DrawRotaGraph(statusbar[num].x + 20, statusbar[num].y + 20, 1.0, 0.0, statusbar[num].h_icon, true, false);

	// Status meter
	DrawGraph(160 + (num * 320), 0, meterSP[num].h[numsp[num]], TRUE);
	DrawGraph(200 + (num * 320), 0, meterAC[num].h[numac[num]], TRUE);
	DrawGraph(240 + (num * 320), 0, meterRT[num].h[numrt[num]], TRUE);
}
*/


void Player::Draw(Player* player, Pointer* pointer, StatusBar* statusbar)
{
	if (player->f_damage == true)	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255); // On-Damage-Effect
	plyfunc.Draw_Players(player, pointer, statusbar);
	if (player->f_damage == true)	player->f_damage = false;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // Off-Damage-Effect
}


void PlayerFunction::Damage(double point, Player* player)
{
	sound.SE_effect(1);
	player->f_damage = true;
	player->hp -= point;

	if (player->hp <= 0.0)
	{
		player->f_exist = false;
		sound.Stop_Music();
	}
}


double PlayerFunction::ShowPosX(PlayerNum num){
	return player[num].VectorX;
}


double PlayerFunction::ShowPosY(PlayerNum num){
	return player[num].VectorY;
}


double PlayerFunction::ShowRota(PlayerNum num)
{
	return player[num].angle;
}


int PlayerFunction::ShowPlayerType(PlayerNum num)
{
	return player[num].type;
}


int PlayerFunction::ShowWeaponType(PlayerNum num)
{
	return player[num].wepon_type;
}


void PlayerFunction::ChangePlayerPosX(enum PlayerNum num, double pos)
{
	player[num].VectorX = pos;
}


void PlayerFunction::ChangePlayerPosY(enum PlayerNum num, double pos)
{
	player[num].VectorY = pos;
}


void PlayerFunction::ChangePlayerSpd(enum PlayerNum num, double speed)
{
	player[num].speed = speed;
}


void PlayerFunction::ChangeBlockAtkFlag(enum PlayerNum num, bool flag)
{
	player[num].f_blockatk = flag;
}


bool PlayerFunction::ShowHitFlag(double l, double r, double t, double b, PlayerNum num, bool f_exist)
{
	if (!f_exist) return false;

	if (num == First)
	{
		if (player[Second].l < r && player[Second].r > l && player[Second].t < b && player[Second].b > t)
		{
			plyfunc.Damage(player[Second].atk, &player[Second]);
			return true;
		}
	}

	if (num == Second)
	{
		if (player[First].l < r && player[First].r > l && player[First].t < b && player[First].b > t)
		{
			plyfunc.Damage(player[First].atk, &player[First]);
			return true;
		}
	}

	return false;
}


bool PlayerFunction::ShowHitFlag2(double l, double r, double t, double b, double damage)
{
	for (int i = 0; i < 2; i++)
	{
		if (player[i].l < r && player[i].r > l && player[i].t < b && player[i].b > t)
		{
			plyfunc.Damage(damage, &player[i]);
			return true;
		}
	}

	return false;
}


bool PlayerFunction::ShowHitFlag3(double l, double r, double t, double b, double damage, PlayerNum num)
{
	if (player[num].l < r && player[num].r > l && player[num].t < b && player[num].b > t)
	{
		plyfunc.Damage(damage, &player[num]);
		return true;
	}

	return false;
}


void PlayerFunction::GetItem(enum Item_Kind ikind, enum PlayerNum num)
{
	if (ikind == HpUp)		itemef.Hitpoint(num);
	if (ikind == AtkUp)		itemef.Attack(num);
	if (ikind == SpUp)		itemef.Speed(num);
	if (ikind == RtUp)		itemef.Rotation(num);
	if (ikind == AcUp)		itemef.Accel(num);
	if (ikind == Recovery)	itemef.Recovery(num);
	if (ikind == Dokuro)	itemef.Dokuro(num);
}


double PlayerFunction::ShowPlayerStatus(PlayerNum num, PlayerStatusItem kind)
{
	if (kind == Attack) return iatk[num];
	
	return -1.0;
}


void P_ItemEffect::Hitpoint(PlayerNum num)
{
	sound.SE_effect(7);
	imaxhp[num] += hpup;
	player[num].hp = player[num].Maxhp + imaxhp[num];
}


void P_ItemEffect::Attack(PlayerNum num)
{
	sound.SE_effect(2);
	iatk[num] += atkup;
	if (iatk[num] > atkup * 3.0) iatk[num] = atkup * 3.0;
}


void P_ItemEffect::Speed(PlayerNum num)
{
	sound.SE_effect(2);
	imaxspd[num] += spup;
	if (imaxspd[num] > spup * 5.0) imaxspd[num] = spup * 5.0;
	else numsp[num]++;
}


void P_ItemEffect::Rotation(PlayerNum num)
{
	sound.SE_effect(2);
	irota[num] += rtup;
	if (irota[num] > rtup * 5.0) irota[num] = rtup * 5.0;
	else numrt[num]++;
}


void P_ItemEffect::Accel(PlayerNum num)
{
	sound.SE_effect(3);
	iaccel[num] += acup;
	if (iaccel[num] > acup * 5.0) iaccel[num] = acup * 5.0;
	else numac[num]++;
}


void P_ItemEffect::Recovery(PlayerNum num)
{
	sound.SE_effect(6);
	player[num].hp += 2.0;
	if (player[num].hp > player[num].Maxhp + imaxhp[num])	player[num].hp = player[num].Maxhp + imaxhp[num]; // 最大HPを上回らない
}


void P_ItemEffect::Dokuro(PlayerNum num)
{
	sound.SE_effect(9);
	SwapposiX = player[First].VectorX;
	SwapposiY = player[First].VectorY;
	player[First].VectorX = player[Second].VectorX;
	player[First].VectorY = player[Second].VectorY;
	player[Second].VectorX = SwapposiX;
	player[Second].VectorY = SwapposiY;
}


bool FinishGame(){
	return f_IsFinish;
}


enum PlayerNum ShowWinnerPlayer()
{
	if (player[First].f_exist == false)
		return Second;
	if (player[Second].f_exist == false)
		return First;

	return First;
}