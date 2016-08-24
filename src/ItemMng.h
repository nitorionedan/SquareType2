#pragma once

#include <vector>
#include "Task.h"
#include "PlayerMng.h"

using namespace std;

enum Item_Kind{
	// Status Up
	HpUp,  // �ϋv�̓A�b�v
	AtkUp, // �U���̓A�b�v
	SpUp,  // �ō����A�b�v
	RtUp,  // ����A�b�v
	AcUp,  // �����x�A�b�v

	// Other
	Recovery,
	Dokuro,   // Swap, Death, etc...

	// Weapon
	iBuster,
	iLaser,
	iWay,
	iWave,
	iMissile,
	iAllRange
};


class ItemMng{
public:
	void Initialize();
	void InitGraph();
	void Finalize();
	void Update();
	void Draw();
};


class Item{
public:
	int h[10];
	int kind; // ��� [enum]:HpUp, AtkUp, SpUp, RtUp,
	double x, y;
	double l, r, t, b;
	int c_h; // AnimeTimeCounter

	bool f_exist;

	bool ShowHitTrigger();
	void ChangeFlag();
};



class ItemIcon{
public:
	double x, y;
	vector<double> vx, vy;
	vector<int> vh;
	int kind;
};


class ItemFunction{
public:
	bool ShowIsHit();
	void ChangeExistFlag(double x, double y);
	void  CheckHit(double l, double r, double t, double b, enum PlayerNum num);
};