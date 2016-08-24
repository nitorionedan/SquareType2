#pragma once
#pragma warning(disable:4786)

#include <vector>
#include <deque>
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "PlayerMng.h"

using namespace std;

// These are the number of bullets.
static const int redNum  = 4;
static const int blueNum = 10;


enum BulletType{
	Buster,
	Laser,
	Wave,
	Missile,
	Bomb,
	Way,
	All
};


class BulletMng{
public:
	void InitGraphs();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	void Init_blt(enum PlayerNum);
	void Update_blt(PlayerNum);
	void Draw_blt(PlayerNum);
};


class Bltdate{
public:
	vector<int>    vh;
	vector<double> x, y;
	vector<double> rota;
	vector<double> x_fire, y_fire;
	vector<double> rota_fire;
	vector<double> l, r, t, b;
	deque<bool>    f_exist; // 「vector<bool> はかなり厄介だったため deque<bool> にしました」
};


class BulletGraph{
public:
	vector<int> vh_ef; // エフェクト

	vector<int> vh_buster;
	vector<int> vh_lasser;
	vector<int> vh_wave;
	vector<int> vh_missile;
};


class Bullet{
public:
	double Maxspeed;
	double speed;
	double atk;
};


// Buster
class Buster : public Bltdate{
public:
	void Update(enum PlayerNum);
	void Draw(enum PlayerNum);
	void Fire(enum PlayerNum);
	void Ready(enum PlayerNum);
	void Move(enum PlayerNum);
	void HitCheck(enum PlayerNum);
	void Reset(enum PlayerNum, int);
};


// Laser
class Laser : public Bltdate{
public:
	void Update();
	void Draw();
};