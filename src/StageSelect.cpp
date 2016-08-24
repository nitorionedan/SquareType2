#include "DxLib.h"
#include "StageSelect.h"
#include "vector"
#include "Keyboard.h"
#include "Sound.h"
#include "Stage.h"
#include "StageDate.h"


using namespace std;


static class StageSelect_Graph graph;
static class SelectPointer     stageptr;
static class Sound_Func        soundf;
static class SelectBack		   back;


static int SelectNum;
static int h_back; // Åustage selectÅv


static const int StageAllNum = 8;

static const int AniPtrTime = 120;
static const int AniPtrFirstTime = AniPtrTime - 1;
static const int AniPtrAllNum = sizeof(stageptr.h) / sizeof(stageptr.h[0]);

StageSelect::StageSelect(ISceneChanger* changer) :BaseScene(changer){}


void StageSelect::Initialize()
{
	SelectNum = 0;
	stageptr.counter = 0;
	stageptr.c_h = 0;

	soundf.Play_Music(10);
}


void StageSelect::InitGraphs()
{
	graph.vh.push_back(LoadGraph("GRAPH/GAME/ForestStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/FireStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/IceStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/RuinsStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/MachineStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/MysteryStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/SpaceStage.png"));
	graph.vh.push_back(LoadGraph("GRAPH/GAME/SeaStage.png"));

	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/forest.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/fire.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/ice.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/ruins.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/machine.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/mystery.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/space.png"));
	graph.vh_back.push_back(LoadGraph("GRAPH/GAME/STAGE/sea.png"));

	LoadDivGraph("GRAPH/GAME/stageptr.png", 5, 5, 1, 18, 35, stageptr.h); // 90x35

	h_back = LoadGraph("GRAPH/GAME/stageselect_title.png");
}


void StageSelect::Finalize()
{
	for (auto itr = graph.vh.begin(); itr < graph.vh.end(); ++itr) DeleteGraph(*itr);
	for (int i = 0; i < 10; i++)	DeleteGraph(stageptr.h[i]);
	graph.vh.clear();
	graph.vh_back.clear();
}


void StageSelect::Update()
{
	stageptr.counter++;
	stageptr.c_h = stageptr.counter % AniPtrTime * AniPtrAllNum;

	if (Keyboard_Get(LEFT) == 1 || Keyboard_Get(RIGHT) == 1) soundf.SE(0);
	if (Keyboard_Get(LEFT) == 1)	SelectNum = (SelectNum + 1) % StageAllNum;
	if (Keyboard_Get(RIGHT) == 1)	SelectNum = (SelectNum + (StageAllNum-1)) % StageAllNum;

	if (Keyboard_Get(KEY_INPUT_Z) == 1 || Keyboard_Get(ENTER) == 1)
	{
		soundf.Stop_Music();

		if (SelectNum == 0)
		{
			GetNowStageType(Forest);
			GetStageNum(Forest00, 2, ShowStageType());
		}
		if (SelectNum == 1)
		{
			GetNowStageType(Fire);
			GetStageNum(Fire00, 5, ShowStageType());
		}
		if (SelectNum == 2)
		{
			GetNowStageType(Ice);
			GetStageNum(Ice00, 4, ShowStageType());
		}
		if (SelectNum == 3)
		{
			GetNowStageType(Ruin);
			GetStageNum(Ruin00, 3, ShowStageType());
		}
		if (SelectNum == 4)
		{
			GetNowStageType(Machine);
			GetStageNum(Machine00, 12, ShowStageType());
		}
		if (SelectNum == 5)
		{
			GetNowStageType(Mystery);
			GetStageNum(Mystery00, 7, ShowStageType());
		}
		if (SelectNum == 6)
		{
			GetNowStageType(Space);
			GetStageNum(Space00, 8, ShowStageType());
		}
		if (SelectNum == 7)
		{
			GetNowStageType(Sea);
			GetStageNum(Sea00, 6, ShowStageType());
		}

		mSceneChanger->ChangeScene(eScene_Game);
	}

	if (Keyboard_Get(KEY_INPUT_X) == 1)
	{
		soundf.Stop_Music();
		mSceneChanger->ChangeScene(eScene_ShipSelect);
	}
}


void StageSelect::Draw()
{
	DrawRotaGraph(320, 240, 2.0, 0.0, h_back, FALSE, FALSE);

	DrawRotaGraph(320, 240, 2.0, 0.0, back.h, TRUE, FALSE);
	DrawRotaGraph(320, 240, 0.9, 0.0, graph.vh_back[SelectNum], TRUE, FALSE);
	DrawRotaGraph(320, 240, 2.0, 0.0, graph.vh[SelectNum],      TRUE, FALSE);

	for (int i = 0; i < AniPtrAllNum; i++)
	{
		if (stageptr.c_h >= i * AniPtrTime && stageptr.c_h <= AniPtrFirstTime + (i * AniPtrTime))
		{
			DrawRotaGraph(100, 240, 2.0, 0.0, stageptr.h[i], TRUE, FALSE);
			DrawRotaGraph(540, 240, 2.0, 0.0, stageptr.h[i], TRUE, TRUE);
		}
	}
}