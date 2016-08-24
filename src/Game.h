/* Game.h */
// All you need is debug ...

#pragma once

#include "BaseScene.h"
#include "ISceneChanger.h"


class Game : public BaseScene{
public:
	Game::Game(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};