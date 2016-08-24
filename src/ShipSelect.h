#pragma once

#include "BaseScene.h"
#include "ISceneChanger.h"
#include <vector>


class ShipSelect : public BaseScene{
public:
	ShipSelect::ShipSelect(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};


class ShipSelect_Graph{
public:
	int h;
	
	int x, y;
	double dx, dy;
};


class ShipPtr{
public:
	int h[2];
	int x[2], y[2];
	int MasX[2], MasY[2];

	bool f_IsSelect[2];
};


class Sign{
public:
	int h[2];
	int x[2], y[2];
};


enum PlayerType GetPlayerType(enum PlayerNum);