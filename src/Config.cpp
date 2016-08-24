/* Config.cpp */

#include "DxLib.h"
#include "Config.h"
#include "Sound.h"
#include "Keyboard.h"
#include "SceneMng.h"
#include "Menu.h"


static bool window;


//プログラム＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

Config::Config(ISceneChanger* changer):BaseScene(changer)
{}

void Config::InitGraphs()
{
}

//初期化
void Config::Initialize()
{
	window = true;
}

void Config::Finalize()
{

}

//更新
void Config::Update()
{

}

//描画
void Config::Draw()
{
}