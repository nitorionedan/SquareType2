#pragma once

#include <vector>
#include "BulletMng.h"
#include "ItemMng.h"

using namespace std;

// 1P or 2P : (First, Second)
enum PlayerNum{
	First, // 1P
	Second // 2P
};

// (Type of space ship.)
enum PlayerType{
	Normal,
	Speedy, // missile
	Attacky, // Attacky ? nanda sore HAHAHA!fuck
	Blue,
	Purple,
	Black,
	Orange
};


enum PlayerStatusItem{
	Maxhp,
	Attack,
	Maxspeed,
	Rota,
	Acce // : Accel
};


class PlayerMng{
public:
	void Initialize();
	void InitGraph();
	void Finalize();
	void Update();
	void Draw();
};


class Graph_Date
{
public:
	Graph_Date(){}
	~Graph_Date()
	{
		for (auto ary : vh_red)		DeleteGraph(ary);
		for (auto ary : vh_blue)	DeleteGraph(ary);
		for (auto ary : vh_purple)	DeleteGraph(ary);
		for (auto ary : vh_yellow)	DeleteGraph(ary);
		for (auto ary : vh_green)	DeleteGraph(ary);
		for (auto ary : vh_orange)	DeleteGraph(ary);
		for (auto ary : vh_black)	DeleteGraph(ary);

		vh_red.clear();
		vh_blue.clear();
		vh_purple.clear();
		vh_statusbar.clear();
		vh_zanki.clear();
		vh_black.clear();
	}

	double x, y;
	vector<int> vh_red;
	vector<int> vh_blue;
	vector<int> vh_purple;
	vector<int> vh_yellow;
	vector<int> vh_green;
	vector<int> vh_orange;
	vector<int> vh_black;

	vector<int> vh_statusbar;
	vector<int> vh_powmem;
	vector<int> vh_hpmem;
	vector<int> vh_zanki;
};


class Player : public Graph_Date{
public:
	int    h;
	int    type; // [enum] Normal, Speedy, Attacky, Laser
	int wepon_type; // enum
	double VectorX, VectorY;
	double speed;
	double Maxspeed;
	double angle;
	double rota; // 定数にしたい
	double Accel;
	double Inertia;
	double Fric;

	double Maxhp;
	double hp;
	double atk;
	int    zanki;
	double l, r, t, b; // Hit Range

	bool f_exist;
	bool f_blockatk; // ブロックに衝突したときダメージを与えるかどうか
	bool f_damage;

	void Draw(class Player*, class Pointer*, class StatusBar*);
};


class PlayerFunction{
public:
	void Initialize_Type(const PlayerNum, const PlayerType);
	void Update_Players(const PlayerNum);
	void Draw_Players   (class Player*, class Pointer*, class StatusBar*);
	void Damage  (double point, class Player*);
	void StatusUp(PlayerNum num, int itemtype);

	bool   ShowHitFlag(double l, double r, double t, double b, const PlayerNum, bool f_exist); // 弾とか
	bool   ShowHitFlag2(double l, double r, double t, double b, double damage); // 弾とか
	bool   ShowHitFlag3(double l, double r, double t, double b, double damage, const PlayerNum);
	double ShowPosX(const PlayerNum);
	double ShowPosY(const PlayerNum);
	double ShowRota(const PlayerNum);
	int    ShowPlayerType(const PlayerNum);
	int    ShowWeaponType(const PlayerNum);
	double ShowPlayerStatus(PlayerNum, PlayerStatusItem);

	void ChangePlayerType(const PlayerType); // Please use original enum.
	void ChangeWeapon(int);
	void ChangePlayerPosX(PlayerNum, double pos);
	void ChangePlayerPosY(PlayerNum, double pos);
	void ChangePlayerSpd(PlayerNum, double speed); // change speed
	void ChangeBlockAtkFlag(PlayerNum, bool flag);

	void GetItem(enum Item_Kind, PlayerNum);
};


class P_ItemEffect{
public:
	void Hitpoint(PlayerNum);
	void Attack(PlayerNum);
	void Speed(PlayerNum);
	void Rotation(PlayerNum);
	void Accel(PlayerNum);
	void Recovery(PlayerNum);
	void Dokuro(PlayerNum);
};


class Pointer : public Graph_Date{
public:
	int h[8]; // 上0, 2, 4, 6 : 下1, 3, 5, 7
	int c_h;  // 画像ハンドルのためのカウンター
	bool f_posi; // true : 上, false : 下
};


class StatusGraph{
public:
	int x, y;
	int counter; // アニメ用カウンター
};


class StatusBar : public StatusGraph{
public:
	int h;
	int h_icon;
};


class HPmemory : public StatusGraph{
public:
	int h[2]; // on off
	bool f_light; // 光ってるかどうか
};


class PowerMemory : public StatusGraph{
public:
	int h;
};


class Zanki : public StatusGraph{
public:

};


class Meter : public Graph_Date{
public:
	int x, y;
	int h[6];
};


bool FinishGame();
enum PlayerNum ShowWinnerPlayer();