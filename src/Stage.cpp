#pragma warning(disable:4244)


#include <vector>
#include <cmath>
#include "DxLib.h"
#include "Stage.h"
#include "Keyboard.h"
#include "Sound.h"
#include "PlayerMng.h"
#include "StageDate.h"
#include "ItemMng.h"

using namespace std;

static class Block block[BlockNum], vblock, viceblock, vfireblock, vseablock, vspaceblock, vruinblock, vforestblock, vmysteryblock;
static class EnemyBlock EBlock;
static class ShotBlock eBlock[BlockNum];
static class Stage stage;
static class BlockFunction blkfunc;
static class Sound_Func soundf;
static class PlayerFunction plyfunc;
static class Sound_Func sound;
static class ItemFunction itemfunc;
static class Stage_2D backview; // 背景用

static int c_blocknum;
static int c_eblockNum;
static int h_backsample;
static int NowStageType;
static int itemrand; // アイテムをドロップするか否か...
static int SearchSpeedX[BlockNum], SearchSpeedY[BlockNum]; // ShotBlockのターゲットラインの速さ


static const int eReadyTime = 300; // To EnemyBlock.
static const double Pai = 3.14159265359;


void Stage::InitGraph()
{
	blkfunc.InitGraph();
	h_backsample = LoadGraph("GRAPH/BACKVIEW/test_tuto.png");

	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/machine.png"));
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/fire.png"));
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/ice.png"));
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/sea.png"));
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/forest.png")); // 4
	
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/space.png"));
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/mystery.png"));
	backview.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/ruins.png"));
}


void BlockFunction::InitGraph()
{
	// 1 ~ 9種類のブロックだが、一つに名コマも入るためもっと必要
	vblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/block00.png"));
	vblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/block01_2.png"));
	vblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/block02.png"));
	vblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/corner.png"));
	vblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/shotblock.png"));

	viceblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/iceblock00.png"));
	viceblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/iceblock01.png"));
	viceblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/iceblock02.png"));
	viceblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/iceblock03.png"));

	vruinblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/ruinblock00.png"));
	vruinblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/ruinblock01.png"));
	vruinblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/ruinblock02.png"));
	vruinblock.vh.push_back(LoadGraph("GRAPH/GAME//STAGE/ruinblock03.png"));

	vforestblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/forestblock00.png"));
	vforestblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/forestblock01.png"));
	vforestblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/forestblock02.png"));
	vforestblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/forestblock03.png"));

	vfireblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/fireblock00.png"));
	vfireblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/fireblock01.png"));
	vfireblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/fireblock02.png"));
	vfireblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/fireblock03.png"));

	vseablock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/seablock00.png"));
	vseablock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/seablock01.png"));
	vseablock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/seablock02.png"));
	vseablock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/seablock03.png"));

	vmysteryblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/mysteryblock00.png"));
	vmysteryblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/mysteryblock01.png"));
	vmysteryblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/mysteryblock02.png"));
	vmysteryblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/mysteryblock03.png"));

	vspaceblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/spaceblock00.png"));
	vspaceblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/spaceblock01.png"));
	vspaceblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/spaceblock02.png"));
	vspaceblock.vh.push_back(LoadGraph("GRAPH/GAME/STAGE/spaceblock03.png"));
}


void Stage::Initialize()
{
	c_blocknum = 0;

	SRand(GetNowCount()); // Sheed of random.

	// 当たり範囲
	for (int i = 0; i < BlockNum; i++)
	{
		block[i].l = block[i].x - 10.0;
		block[i].r = block[i].x + 10.0;
		block[i].t = block[i].y - 10.0;
		block[i].b = block[i].y + 10.0;

		eBlock[i].f_exist = false;
		eBlock[i].readyTime = eReadyTime;
		eBlock[i].SearchLineX = GetRand(640);
		eBlock[i].SearchLineY = GetRand(480);
		SearchSpeedX[i] = 1.5;
		SearchSpeedY[i] = 1.5;
	}
}


void Stage::StageSet(int stage[][MapWidth], int musicnum, int Stagetype)
{
	for (int i = 0; i < MapHeight; i++)
	{
		for (int j = 0; j < MapWidth; j++)
		{
			block[c_blocknum].x = (j * BlockSize) + AdjustX;
			block[c_blocknum].y = (i * BlockSize) + AdjustY;

			if (stage[i][j] == 0)
			{
				block[c_blocknum].f_dead = true; // ブロック無し
				block[c_blocknum].f_drop = false;
			}

			if (stage[i][j] != 0)
			{
				block[c_blocknum].f_dead = false;
			}
			if (stage[i][j] == 1)
			{
				block[c_blocknum].kindNum = stage[i][j]; // ブロックの種類
				block[c_blocknum].f_drop = false; // アイテム保有なし

				if (Stagetype == Machine || Stagetype == SampleStage)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/block00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Forest)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/forestblock00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Fire)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/fireblock00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Ruin)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/ruinblock00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Sea)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/seablock00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Space)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/spaceblock00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Ice)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/iceblock00.png");
					block[c_blocknum].hp = 3.0;
				}
				if (Stagetype == Mystery)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/mysteryblock00.png");
					block[c_blocknum].hp = 3.0;
				}
			}
			if (stage[i][j] == 2)
			{
				block[c_blocknum].kindNum = stage[i][j];

				if (Stagetype == Machine || Stagetype == SampleStage)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/corner.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Forest)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/forestblock03.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Fire)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/fireblock03.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Ruin)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/ruinblock03.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Sea)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/seablock03.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Space)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/spaceblock03.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Ice)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/iceblock03.png");
					block[c_blocknum].hp = 100.0;
				}
				if (Stagetype == Mystery)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/mysteryblock03.png");
					block[c_blocknum].hp = 100.0;
				}
			}
			if (stage[i][j] == 3)
			{
				if (Stagetype == Machine || Stagetype == SampleStage)
				{
					block[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/shotblock.png");
					block[c_blocknum].hp = 10.0;
					block[c_blocknum].f_drop = true;

					//　このブロックの弾 情報
					eBlock[c_blocknum].f_dead = false;
					eBlock[c_blocknum].x = -100.0;
					eBlock[c_blocknum].y = -100.0;
					eBlock[c_blocknum].defX = block[c_blocknum].x;
					eBlock[c_blocknum].defY = block[c_blocknum].y;
					eBlock[c_blocknum].h = LoadGraph("GRAPH/GAME/STAGE/eshot.png");
				}
				if (Stagetype == Forest)
				{
				}
				if (Stagetype == Fire)
				{
				}
				if (Stagetype == Ruin)
				{
				}
				if (Stagetype == Sea)
				{
				}
				if (Stagetype == Space)
				{
				}
				if (Stagetype == Ice)
				{
				}
				if (Stagetype == Mystery)
				{
				}
			}
			c_blocknum++;
		}
	}
	c_blocknum = 0;	// reset
	soundf.Play_Music(musicnum);
}


void Stage::Finalize()
{
	DeleteGraph(h_backsample);
	for (auto itr = vblock.vh.begin();     itr < vblock.vh.end();     ++itr) DeleteGraph(*itr);
	for (auto itr = viceblock.vh.begin();  itr < viceblock.vh.end();  ++itr) DeleteGraph(*itr);
	for (auto itr = vruinblock.vh.begin(); itr < vruinblock.vh.end(); ++itr) DeleteGraph(*itr);
	for (auto itr = vfireblock.vh.begin(); itr < vfireblock.vh.end(); ++itr) DeleteGraph(*itr);
	for (auto itr = vseablock.vh.begin(); itr < vseablock.vh.end(); ++itr) DeleteGraph(*itr);
	for (auto itr = vmysteryblock.vh.begin(); itr < vmysteryblock.vh.end(); ++itr) DeleteGraph(*itr);
	for (auto itr = vspaceblock.vh.begin(); itr < vspaceblock.vh.end(); ++itr) DeleteGraph(*itr);
	for (auto itr = vruinblock.vh.begin(); itr < vruinblock.vh.end(); ++itr) DeleteGraph(*itr);
	for (auto itr = backview.vh.begin();   itr < backview.vh.end();   ++itr) DeleteGraph(*itr);
	for (int i = 0; i < BlockNum; i++)	DeleteGraph(block[i].h);

	vblock.vh.clear();
	viceblock.vh.clear();
	vruinblock.vh.clear();
	vseablock.vh.clear();
	vspaceblock.vh.clear();
	vfireblock.vh.clear();
	vforestblock.vh.clear();
	vmysteryblock.vh.clear();

	backview.vh.clear();
}


void Stage::Update()
{
	for (int i = 0; i < BlockNum; i++)	EBlock.Update(i);
}


void Stage::Draw()
{	
	// Backview
	if (ShowStageType() == SampleStage) DrawRotaGraph(320, 260, 2.0, 0.0, h_backsample, TRUE, FALSE);
	// ↓こいつらどうにかせねば(^^;
	if (ShowStageType() == Machine) DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[0], TRUE, FALSE);
	if (ShowStageType() == Fire)	DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[1], TRUE, FALSE);
	if (ShowStageType() == Ice)		DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[2], TRUE, FALSE);
	if (ShowStageType() == Sea)		DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[3], TRUE, FALSE);
	if (ShowStageType() == Forest)	DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[4], TRUE, FALSE);
	if (ShowStageType() == Space)	DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[5], TRUE, FALSE);
	if (ShowStageType() == Mystery) DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[6], TRUE, FALSE);
	if (ShowStageType() == Ruin)	DrawRotaGraph(320, 240, 2.0, 0.0, backview.vh[7], TRUE, FALSE);
	
	// Blocks
	for (int i = 0; i < BlockNum; i++)
	{
		if (block[i].f_hit == true)	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255); // On-Damage-Effect

		if (block[i].f_dead == false)	DrawRotaGraph(block[i].x, block[i].y, 1.0, 0.0, block[i].h, TRUE, FALSE);

		if (block[i].f_hit == true)	block[i].f_hit = false;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // Off-Damage-Effect
	}
	for (int i = 0; i < BlockNum; i++)	EBlock.Draw(i);
}


bool Block::ShowHitTrigger(double l, double r, double t, double b, double atk)
{
	for (int i = 0; i < BlockNum; i++)
	{
		if (l < block[i].r && r > block[i].l && t < block[i].b && b > block[i].t)
		{
			if (block[i].ShowDeadTrigger(i) == false)
			{
				block[i].DamageBlock(i, atk, ShowStageType());
				return true;
			}
		}
	}

	return false;
}


// 機体とブロックが接触で調整
bool Block::ShowHitTrigger2(double l, double r, double t, double b, double speed, enum PlayerNum num)
{
	for (int i = 0; i < BlockNum; i++)
	{
		if (l < block[i].r && r > block[i].l && t < block[i].b && b > block[i].t)
		{
			if (block[i].ShowDeadTrigger(i) == false)
			{
				plyfunc.ChangePlayerSpd(num, 0.0000);

				if (speed >= 4.0 || speed <= -4.0)
				{
					block[i].DamageBlock(i, 1.0, ShowStageType());
					sound.SE_effect(0);
				}
				
				if (plyfunc.ShowPosX(num) < block[i].l)
				{
					plyfunc.ChangePlayerPosX(num, block[i].l - 8.0);
					return true;
				}
				if (plyfunc.ShowPosY(num) < block[i].t)
				{
					plyfunc.ChangePlayerPosY(num, block[i].t - 8.0);
					return true;
				}
				if (plyfunc.ShowPosX(num) > block[i].r)
				{
					plyfunc.ChangePlayerPosX(num, block[i].r + 8.0);
					return true;
				}
				if (plyfunc.ShowPosY(num) > block[i].b)
				{
					plyfunc.ChangePlayerPosY(num, block[i].b + 8.0);
					return true;
				}
			}
		}
	}

	return false;
}


void Block::HitTrigger(double x, double y, double speed, enum PlayerNum num)
{
	for (int i = 0; i < BlockNum; i++)
	{
		if (plyfunc.ShowPosX(num))
		{
			
		}
	}
}


void Block::DamageBlock(int num, int atk, int Stagetype)
{
//	if (block[num].hp == 100.0) return;

	block[num].f_hit = true;

	// ステージによってある程度違いを作る計画
	if (Stagetype == Machine || Stagetype == SampleStage)
	{
		block[num].hp -= atk;
			
		// ノーマルコンテナの場合
		if (block[num].kindNum == 1)
		{
			if (block[num].hp == 2) block[num].h = vblock.vh[1];
			if (block[num].hp == 1) block[num].h = vblock.vh[2];
		}
		if (block[num].hp <= 0)
		{
			soundf.SE_effect(11);
			block[num].f_dead = true;
			EBlock.Delete(num);
			// アイテム出現	
			if (!block[num].f_drop)
			{
				itemrand = GetRand(100);
				if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
			}
			else itemfunc.ChangeExistFlag(block[num].x, block[num].y);
		 }
	}
	if (Stagetype == Forest)
	{	
		block[num].hp -= atk;
		if (block[num].hp == 2) block[num].h = vforestblock.vh[1];
		if (block[num].hp == 1) block[num].h = vforestblock.vh[2];
		if (block[num].hp <= 0)
		{
			block[num].f_dead = true;
			// アイテム出現	
			SRand(GetNowCount());
			itemrand = GetRand(100);
			if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
		}
	}
	if (Stagetype == Fire)
	{
		block[num].hp -= atk;
		if (block[num].hp == 2) block[num].h = vfireblock.vh[1];
		if (block[num].hp == 1) block[num].h = vfireblock.vh[2];
		if (block[num].hp <= 0)
		{
			block[num].f_dead = true;
			// アイテム出現	
			SRand(GetNowCount());
			itemrand = GetRand(100);
			if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
		}
	}
	if (Stagetype == Ruin)
	{
		if (block[num].hp != 100.0)
		{
			block[num].hp -= atk;
			if (block[num].hp == 2) block[num].h = vruinblock.vh[1];
			if (block[num].hp == 1) block[num].h = vruinblock.vh[2];
			if (block[num].hp <= 0)
			{
				block[num].f_dead = true;
				// アイテム出現	
				SRand(GetNowCount());
				itemrand = GetRand(100);
				if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
			}
		}
	}

	if (Stagetype == Sea)
	{
		block[num].hp -= atk;
		if (block[num].hp == 2) block[num].h = vseablock.vh[1];
		if (block[num].hp == 1) block[num].h = vseablock.vh[2];
		if (block[num].hp <= 0)
		{
			block[num].f_dead = true;
			// アイテム出現	
			SRand(GetNowCount());
			itemrand = GetRand(100);
			if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
		}
	}

	if (Stagetype == Space)
	{
		block[num].hp -= atk;
		if (block[num].hp == 2) block[num].h = vspaceblock.vh[1];
		if (block[num].hp == 1) block[num].h = vspaceblock.vh[2];
		if (block[num].hp <= 0)
		{
			block[num].f_dead = true;
			// アイテム出現	
			SRand(GetNowCount());
			itemrand = GetRand(100);
			if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
		}
	}

	if (Stagetype == Ice)
	{
		if (block[num].hp != 100.0)
		{
			block[num].hp -= atk;
			if (block[num].hp == 2) block[num].h = viceblock.vh[1];
			if (block[num].hp == 1) block[num].h = viceblock.vh[2];
			if (block[num].hp <= 0)
			{
				block[num].f_dead = true;
				// アイテム出現	
				SRand(GetNowCount());
				itemrand = GetRand(100);
				if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
			}
		}
	}

	if (Stagetype == Mystery)
	{
		block[num].hp -= atk;
		if (block[num].hp == 2) block[num].h = vmysteryblock.vh[1];
		if (block[num].hp == 1) block[num].h = vmysteryblock.vh[2];
		if (block[num].hp <= 0)
		{
			block[num].f_dead = true;
			// アイテム出現	
			SRand(GetNowCount());
			itemrand = GetRand(100);
			if (itemrand <= 29)	itemfunc.ChangeExistFlag(block[num].x, block[num].y);
		}
	}
}


bool Block::ShowDeadTrigger(const int num)
{
	
	if (block[num].f_dead == true)	return true;

	// なにもぶつかってない
	return false;
}


void GetNowStageType(const int type)
{
	NowStageType = type;
}


int ShowStageType()
{
	return NowStageType;
}


void GetStageNum(int Stage[][MapWidth], int musicnum, int StageType)
{
	stage.StageSet(Stage, musicnum, StageType); // ここではブロックのステータスも設定する
}


void EnemyBlock::Initialize(const int num)
{
	eBlock[num].f_exist = false;
}


void EnemyBlock::Update(const int num)
{
	if (eBlock[num].f_dead)	return; // 本体がなければとばす

	EBlock.Ready(num);

	eBlock[num].SearchLineX += SearchSpeedX[num];
	eBlock[num].SearchLineY += SearchSpeedY[num];

	if (eBlock[num].SearchLineX >= 640.0 || eBlock[num].SearchLineX <= 0.0)
		SearchSpeedX[num] = -SearchSpeedX[num];
	if (eBlock[num].SearchLineY >= 480.0 || eBlock[num].SearchLineY <= 0.0)
		SearchSpeedY[num] = -SearchSpeedY[num];

	// COUNT DOWN
	if (eBlock[num].f_exist == false)
		eBlock[num].readyTime--;
	
	// Preparing OK !
	if (eBlock[num].readyTime < 0)
		eBlock[num].readyTime = 0;
	
	// Lock on.
	if (eBlock[num].readyTime == 0 && eBlock[num].SearchRangeXL <= plyfunc.ShowPosX(First) && eBlock[num].SearchRangeXR >= plyfunc.ShowPosX(First))
		EBlock.Fire(num, First);
	if (eBlock[num].readyTime == 0 && eBlock[num].SearchRangeYT <= plyfunc.ShowPosY(Second) && eBlock[num].SearchRangeYB >= plyfunc.ShowPosY(Second))
		EBlock.Fire(num, Second);
	
	// MOVE.
	EBlock.Move(num);

	// Ring out.
	if (eBlock[num].x > 640.0 || eBlock[num].x < 0.0 || eBlock[num].y > 480.0 || eBlock[num].y < 0.0)
		EBlock.Reset(num);

	if (eBlock[num].f_exist == true)
		EBlock.HitCheck(num);
}


void EnemyBlock::Draw(const int num)
{
	if (eBlock[num].f_dead) return;

	if (eBlock[num].f_exist)
		DrawRotaGraph(eBlock[num].x, eBlock[num].y, 1.0, 0.0, eBlock[num].h, TRUE, FALSE);
}


void EnemyBlock::Ready(const int num)
{
	eBlock[num].SearchLineX += SearchSpeedX[num];
	eBlock[num].SearchLineY += SearchSpeedY[num];
	eBlock[num].SearchRangeXL = eBlock[num].SearchLineX - 1.0;
	eBlock[num].SearchRangeXR = eBlock[num].SearchLineX + 1.0;
	eBlock[num].SearchRangeYT = eBlock[num].SearchLineY - 1.0;
	eBlock[num].SearchRangeYB = eBlock[num].SearchLineY + 1.0;
}


void EnemyBlock::Fire(const int num, const PlayerNum plyNum)
{
	/* ang = atan2(py - ey, px - ex) */

	eBlock[num].readyTime = eReadyTime;
	
	eBlock[num].x = eBlock[num].defX;
	eBlock[num].y = eBlock[num].defY;
	eBlock[num].f_exist = true;
	eBlock[num].angle = atan2(plyfunc.ShowPosY(plyNum) - eBlock[num].y, plyfunc.ShowPosX(plyNum) - eBlock[num].x);
	sound.SE_effect(10);
}


void EnemyBlock::Move(const int num)
{
	if (!eBlock[num].f_exist) return;
	EBlock.SetHitRange(&eBlock[num]);

	eBlock[num].x += cos(eBlock[num].angle) * eBlock[num].speed;
	eBlock[num].y += sin(eBlock[num].angle) * eBlock[num].speed;
}


void EnemyBlock::Reset(const int num)
{
	eBlock[num].x = eBlock[num].y = -100.0;
	eBlock[num].f_exist = false;
}


void EnemyBlock::HitCheck(const int num)
{
	if (plyfunc.ShowHitFlag3(eBlock[num].l, eBlock[num].r, eBlock[num].t, eBlock[num].b, 1.0, First) == true ||
		plyfunc.ShowHitFlag3(eBlock[num].l, eBlock[num].r, eBlock[num].t, eBlock[num].b, 1.0, Second) == true)
		EBlock.Reset(num);
}


void EnemyBlock::Delete(const int num)
{
	if (eBlock[num].f_dead) return;
	eBlock[num].f_dead = true;
}