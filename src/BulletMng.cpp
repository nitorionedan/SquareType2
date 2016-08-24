#pragma warning(disable:4244)

#include <cmath>
#include <vector>

#include "DxLib.h"
#include "BulletMng.h"
#include "PlayerMng.h"
#include "Keyboard.h"
#include "Stage.h"
#include "Sound.h"


using namespace std;


// Definitions
static const double Pai = 3.14159265359;
static const double BusHitRange = 3.0;

// Limited Screen Frame.
static const double ScLeft    = -10.0; // def 6.0
static const double ScRight   = 640.0; // def 630.0
static const double ScTop     = -10.0; // def 9.0
static const double ScBottom  = 480.0; // def 470
static const int	BusterNum = 6;

// Bullets
static class BulletMng bulletmng;
static class BulletGraph graph;
static class Buster buster[2]; // for 2 players
static class Laser laser[2];
static class Bullet bullet[2]; // essential

// Other classes
static class Player player[2]; // 1P, 2P
static class PlayerFunction plyfunc;
static class Stage  stage;
static class Block  block;
static class Sound_Func  sound;


void BulletMng::InitGraphs()
{

}


void BulletMng::Initialize()
{
	graph.vh_ef.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/dmg00.png"));
	graph.vh_ef.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/dmg01.png"));
	graph.vh_ef.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/dmg02.png"));

	graph.vh_buster.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/buster00.png"));
	graph.vh_buster.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/buster01.png"));
	graph.vh_buster.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/buster02.png"));
	graph.vh_buster.push_back(LoadGraph("GRAPH/GAME/PLAYER/Y-BULLET/buster03.png"));

	bulletmng.Init_blt(First);
	bulletmng.Init_blt(Second);
}


void BulletMng::Init_blt(PlayerNum num)
{
	if (plyfunc.ShowWeaponType(num) == Buster)
	{
		for (int i = 0; i < BusterNum; i++) // 6 を　const　に
		{
			buster[num].vh.push_back(graph.vh_buster[0]);
			buster[num].x.push_back(plyfunc.ShowPosX(num));
			buster[num].y.push_back(plyfunc.ShowPosY(num));
			buster[num].rota.push_back(plyfunc.ShowRota(num));

			buster[num].x_fire.push_back(buster[num].x[i]);
			buster[num].y_fire.push_back(buster[num].y[i]);
			buster[num].rota_fire.push_back(buster[num].rota[i]);

			buster[num].f_exist.push_back(false);
			buster[num].l.push_back(buster[num].x[i] - BusHitRange);
			buster[num].r.push_back(buster[num].x[i] + BusHitRange);
			buster[num].t.push_back(buster[num].y[i] - BusHitRange);
			buster[num].b.push_back(buster[num].y[i] + BusHitRange);
		}
		bullet[num].Maxspeed = 10.0;
		bullet[num].speed    = 10.0;
		bullet[num].atk = 1.0;
	}

	if (plyfunc.ShowPlayerType(num) == Laser)
	{}
	// ...etc
}


void BulletMng::Finalize()
{
	for (int i = 0; i < 2; i++)
		for (auto itr = buster[i].vh.begin(); itr < buster[i].vh.end(); ++itr)		DeleteGraph(*itr);
	for (auto itr = graph.vh_ef.begin(); itr < graph.vh_ef.end(); ++itr)			DeleteGraph(*itr);
	for (auto itr = graph.vh_buster.begin(); itr < graph.vh_buster.end(); ++itr)	DeleteGraph(*itr);

	for (int i = 0; i < 2; i++)	buster[i].vh.clear();
	graph.vh_ef.clear();
	graph.vh_buster.clear();
}


void BulletMng::Update()
{
	bulletmng.Update_blt(First);
	bulletmng.Update_blt(Second);
}


void BulletMng::Update_blt(PlayerNum num){
	if (plyfunc.ShowWeaponType(num) == Buster)	buster[num].Update(num);
}


void BulletMng::Draw()
{
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	bulletmng.Draw_blt(First);
	bulletmng.Draw_blt(Second);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}


void BulletMng::Draw_blt(PlayerNum num)
{
	if (plyfunc.ShowWeaponType(num) == Buster)
		buster[num].Draw(num);
}


void Buster::Update(PlayerNum num)
{
	// 座標などを随時記録し発射準備
	buster[num].Ready(num);

	// 入力で発射されていない弾を発射OKにする
	// 1P
	if (Keyboard_Get(SPACE) == 1 && num == First)	buster[num].Fire(num);
	// 2P
	if (Keyboard_Get(ENTER) == 1 && num == Second)	buster[num].Fire(num);

	// 発射OKな弾は無慈悲に進みだす...そう、彼らに慈悲はない。
	buster[num].Move(num);

	// 当たり判定
	buster[num].HitCheck(num);
}


void Buster::Draw(PlayerNum num)
{
	for (int i = 0; i < BusterNum; i++)
	{
		if (buster[num].f_exist[i] == true){
			DrawRotaGraph(buster[num].x[i], buster[num].y[i], 1.0, (buster[num].rota[i] + Pai / 2), graph.vh_buster[static_cast<int>(plyfunc.ShowPlayerStatus(num, Attack))], TRUE, FALSE);
		}
	}

	for (int i = 0; i < BusterNum; i++){
		DrawFormatString(0, 60 + (i * 20), GetColor(0, 255, 0), "angle[%d] = %lf", i, buster[0].rota[i]);
	}
}


void Buster::Fire(PlayerNum num)
{
	for (int i = 0; i < BusterNum; i++)
	{
		if (buster[num].f_exist[i] == false)
		{
			sound.SE_shot(0);
			buster[num].x[i] = buster[num].x_fire[i];
			buster[num].y[i] = buster[num].y_fire[i];
			buster[num].rota[i] = buster[num].rota_fire[i];
			buster[num].f_exist[i] = true;

			break;
		}
	}
}


void Buster::Ready(const PlayerNum num)
{
	for (int i = 0; i < BusterNum; i++)
	{
		if (buster[num].f_exist[i] == false)
		{
			buster[num].x_fire[i] = plyfunc.ShowPosX(num);
			buster[num].y_fire[i] = plyfunc.ShowPosY(num);
			buster[num].rota_fire[i] = plyfunc.ShowRota(num);
		}
	}
}


void Buster::Move(const PlayerNum num)
{
	for (int i = 0; i < BusterNum; i++)
	{
		if (!buster[num].f_exist[i])	continue;

		// Hit-Range
		buster[num].l[i] = buster[num].x[i] - BusHitRange;
		buster[num].r[i] = buster[num].x[i] + BusHitRange;
		buster[num].t[i] = buster[num].y[i] - BusHitRange;
		buster[num].b[i] = buster[num].y[i] + BusHitRange;

		buster[num].x[i] += cos(buster[num].rota[i]) * bullet[num].speed;
		buster[num].y[i] += sin(buster[num].rota[i]) * bullet[num].speed;
	}
}


void Buster::HitCheck(const PlayerNum num)
{
	for (int i = 0; i < BusterNum; i++)
	{
		// HIT
		if (plyfunc.ShowHitFlag(buster[num].l[i], buster[num].r[i], buster[num].t[i], buster[num].b[i], num, buster[num].f_exist[i]) == true)
		{
			buster[num].Reset(num, i);
			return;
		}

		// ブロックに当たるとリセット
		if (block.ShowHitTrigger(buster[num].l[i], buster[num].r[i], buster[num].t[i], buster[num].b[i], plyfunc.ShowPlayerStatus(num, Attack) + 1.0) == true && buster[num].f_exist[i] == true)
		{
			sound.SE_effect(4);
			buster[num].Reset(num, i);
			return;
		}

		// 画面外でリセット
		if (buster[num].x[i] < ScLeft || buster[num].x[i] > ScRight || buster[num].y[i] < ScTop || buster[num].y[i] > ScBottom)
		{
			buster[num].Reset(num, i);
			return;
		}
	}
}


void Buster::Reset(PlayerNum num, int i)
{
	buster[num].x[i] = plyfunc.ShowPosX(num);
	buster[num].y[i] = plyfunc.ShowPosY(num);
	buster[num].rota[i] = plyfunc.ShowRota(num);
	buster[num].f_exist[i] = false;

	buster[num].l[i] = -100.0;
	buster[num].r[i] = -100.0;
	buster[num].t[i] = -100.0;
	buster[num].b[i] = -100.0;
}