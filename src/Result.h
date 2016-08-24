#pragma once
#include "BaseScene.h"

class Result : public BaseScene{

public:
	Result(ISceneChanger* changer);
	void InitGraphs() override;
	void Initialize() override;		//初期化処理をオーバーライド
	void Finalize() override;		//終了処理をオーバーライド
	void Update() override;			//更新処理をオーバーライド
	void Draw() override;			//描画処理をオーバーライド
};