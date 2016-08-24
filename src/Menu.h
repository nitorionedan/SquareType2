#pragma once

#include "BaseScene.h"


enum Kind{
	Up,
	Middle,
	Under,
	None
};

enum KeyComand{
	UpArrow,
	DownArrow,
	NoneCmd
};


//メニュー画面クラス
class Menu : public BaseScene{
public:
	Menu(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};


class Menu_Functions{
};


class Menu_Graph{
public:
	int h;
	int x, y;
};


class Menu_Select{
private:
	int poskind;

public:
	enum Kind kind[4];
	int  menuNum;
	int  h[4];
	int  x[4];
	double y[4];
	double exrate[4];

	int ShowKind(enum Kind kind)
	{
		poskind = kind;
		return poskind;
	}
	void Move(const KeyComand);
};


void Menu_Initialize();
int  Menu_Quit();
void Menu_Exit();