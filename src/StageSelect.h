#pragma once

#include "BaseScene.h"
#include "ISceneChanger.h"
#include "vector"


using namespace std;


static const int StageNum = 8; // ステージの種類の総数


class StageSelect : public BaseScene{
public:
	StageSelect::StageSelect(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};


class StageSelect_Graph
{
public:
	double x[StageNum], y[StageNum];
	vector<int> vh;
	vector<int> vh_back;
};


class SelectPointer{
public:
	int h[5];
	vector<int> vh;
	int counter;
	int c_h;
};


class SelectBack{
public:
	int h;
};