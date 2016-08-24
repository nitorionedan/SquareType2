#pragma once

#include <vector>
#include "ItemMng.h"
#include "PlayerMng.h"
#include "StageDate.h"

using namespace std;


static const int AdjustX = 10, AdjustY = 50;

static const int BlockSize = 20;
static const int BlockNum = MapWidth * MapHeight;
static const double HitRange = 3.0;


enum StageType{
	Space,
	Fire,
	Sea,
	Forest,
	Ruin,
	Machine,
	Ice,
	Mystery,
	SampleStage
};


enum Attitude{
	Hp3,
	Hp2,
	Hp1
};


enum Kind_Stage{
	One,
	Two,
	Three,
	Four
};


class Stage {
public:
	void InitGraph();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	
	void StageSet(int stage[][MapWidth], int musicnum, int Stagetype); //	ステージの情報をセットする
};


class Stage_2D{
public:
	int x, y;
	int h;
	vector<int> vh;
	double angle;
};


class Block : public Stage_2D{
public:
	int  hhp[3];
	vector<int> vh;
	double  hp;
	int  content; // [e] None, etc...
	bool f_dead, f_drop, f_hit = false;
	int  atti; // [enum] Attitude
	double  l, r, t, b; // Left, Right, Top, Bottom 当たり範囲
	int kindNum; // 種類

	void Damage(int point, int blocknum);
	void DamageBlock(int num, int atk, int Stagetype);
	bool ShowDeadTrigger(int num);
	bool ShowHitTrigger(double l, double r, double t, double b, double atk);
	bool ShowHitTrigger2(double l, double r, double t, double b, double speed, enum PlayerNum num); // スピード判定
	void HitTrigger(double x, double y, double speed, enum PlayerNum num); // スピード判定改
};


class BlockFunction{
public:
	void InitGraph();
};


class EnemyBlock{
public:
	int h;
	int readyTime; // 充填時間
	double x, y;
	double defX, defY; // リセットするとき
	double SearchLineY, SearchLineX; // Lock on.
	double SearchRangeXL, SearchRangeXR; // 幅
	double SearchRangeYT, SearchRangeYB;
	double l, r, t, b;
	double angle, speed = 4.0;
	bool   f_dead = true;
	bool   f_exist;

	void Initialize(const int);
	void Update(const int);
	void Draw(const int);
	void Ready(const int);
	void Fire(const int, const PlayerNum);
	void Move(const int);
	void Reset(const int);
	void HitCheck(const int);
	void Delete(const int);

	void SetHitRange(class EnemyBlock* eblock)
	{
		eblock->l = eblock->x - HitRange;
		eblock->r = eblock->x + HitRange;
		eblock->t = eblock->y - HitRange;
		eblock->b = eblock->y + HitRange;
	}
};

class ShotBlock : public EnemyBlock{};


void GetNowStageType(const int type);
int ShowStageType();
void GetStageNum(int stage[][MapWidth], int musicnum, int StageType);