/* Config.cpp */

#include "DxLib.h"
#include "Config.h"
#include "Sound.h"
#include "Keyboard.h"
#include "SceneMng.h"
#include "Menu.h"


static bool window;


//�v���O������������������������������������������������������������������������������������������������������������������������������������������������������

Config::Config(ISceneChanger* changer):BaseScene(changer)
{}

void Config::InitGraphs()
{
}

//������
void Config::Initialize()
{
	window = true;
}

void Config::Finalize()
{

}

//�X�V
void Config::Update()
{

}

//�`��
void Config::Draw()
{
}