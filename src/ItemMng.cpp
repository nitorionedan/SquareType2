#pragma warning(disable:4244)

#include "DxLib.h"
#include "ItemMng.h"
#include "PlayerMng.h"


static class Item item[10];     // ItemFrame
static class ItemIcon icon[10]; // Icon
static class ItemIcon icon_gra; // Icon画像
static class PlayerFunction plyfunc;

static int i, j; // iterator
static int itemrand;

static const int ItemMaxNum = sizeof(item) / sizeof(item[0]);

static const int ItemTime = 70; // １コマ当たりのフレーム数
static const int ItemFirstFrame = ItemTime - 1; // -> Animation
static const int ItemFrameNum = sizeof(item[0].h) / sizeof(item[0].h[0]); // 全コマ数

static const double ItemHitRange = 9.0;

static int counter; // Animation counter


void ItemMng::Initialize()
{
	counter = 0;
	srand(GetNowCount()); // ランダムの種

	for (i = 0; i < ItemMaxNum; i++)
	{
		item[i].f_exist = false;
		item[i].c_h = 0;
		item[i].x = -100;
		item[i].y = -100;

		icon[i].x = item[i].x;
		icon[i].y = item[i].y;
	}
}


void ItemMng::InitGraph()
{
	for (i = 0; i < ItemMaxNum; i++)	LoadDivGraph("GRAPH/GAME/ITEM/item00.png", 10, 5, 2, 20, 20, item[i].h);

	// Weapon change
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/buster00.png"));

	// Status up
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/hpup.png"));
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/atkup.png"));
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/spup.png"));
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/rtup.png"));
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/acup.png"));
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/recovery.png"));
	icon_gra.vh.push_back(LoadGraph("GRAPH/GAME/ITEM/dokuro.png"));
}


void ItemMng::Finalize()
{
	for (auto itr = icon_gra.vh.begin(); itr < icon_gra.vh.end(); ++itr) DeleteGraph(*itr);
	for (int i = 0; i < ItemMaxNum; i++)	DeleteGraph(item[0].h[i]);
	for (int i = 0; i < ItemMaxNum; i++)	DeleteGraph(item[1].h[i]);

	icon_gra.vh.clear();
}


void ItemMng::Update()
{
	counter++;
	for (i = 0; i < ItemMaxNum; i++)
		item[i].c_h = counter % ItemTime * ItemMaxNum;
}


void ItemMng::Draw()
{
	for (int i = 0; i < ItemMaxNum; i++)
	{
		if (item[i].f_exist == true)
		{
			// Box animation
			for (j = 0; j < ItemFrameNum; j++)
			{
				if (item[i].c_h >= j * ItemTime && item[i].c_h <= ItemFirstFrame + (j * ItemTime))
					DrawRotaGraph(item[i].x, item[i].y, 1.0, 0.0, item[i].h[j], true, false);
			}

			// Icon
			// Weapon
			if (icon[i].kind == iBuster)	DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[0], true, false);
			
			// Status up
			if (icon[i].kind == HpUp)		DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[1], true, false);
			if (icon[i].kind == AtkUp)		DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[2], true, false);
			if (icon[i].kind == SpUp)		DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[3], true, false);
			if (icon[i].kind == RtUp)		DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[4], true, false);
			if (icon[i].kind == AcUp)		DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[5], true, false);
			if (icon[i].kind == Recovery)	DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[6], true, false);
			if (icon[i].kind == Dokuro)		DrawRotaGraph(icon[i].x, icon[i].y, 1.0, 0.0, icon_gra.vh[7], true, false);
		}
	}
}


void ItemFunction::ChangeExistFlag(double x, double y)
{
	itemrand = GetRand(100);
	 
	// 20% の確率で武器アイテム
	if (itemrand < 19)
	{}

	// 80% の確率でステータス系アイテム
	else
	{
		for (i = 0; i < ItemMaxNum; i++)
		{
			if (item[i].f_exist == false)
			{
				SRand(GetNowCount() + itemrand);
				itemrand = GetRand(100);
				if (itemrand >=  0 && itemrand < 19)	icon[i].kind = HpUp;
				if (itemrand >= 20 && itemrand < 33)	icon[i].kind = AtkUp;
				if (itemrand >= 34 && itemrand < 47)	icon[i].kind = SpUp;
				if (itemrand >= 48 && itemrand < 61)	icon[i].kind = RtUp;
				if (itemrand >= 62 && itemrand < 75)	icon[i].kind = AcUp;
				if (itemrand >= 76 && itemrand < 89)	icon[i].kind = Recovery;
				if (itemrand >= 90 && itemrand < 99)	icon[i].kind = Dokuro;

				item[i].x = x;
				item[i].y = y;
				item[i].l = item[i].x - ItemHitRange;
				item[i].r = item[i].x + ItemHitRange;
				item[i].t = item[i].y - ItemHitRange;
				item[i].b = item[i].y + ItemHitRange;
				item[i].kind = icon[i].kind;

				icon[i].x = item[i].x;
				icon[i].y = item[i].y;

				item[i].f_exist = true;
				break;
			}
		}
	}
}


void ItemFunction::CheckHit(double l, double r, double t, double b, enum PlayerNum num)
{
	for (i = 0; i < ItemMaxNum; i++)
	{
		if (item[i].l < r && item[i].r > l && item[i].t < b && item[i].b > t)
		{
			if (item[i].f_exist == true)
			{
				plyfunc.GetItem((Item_Kind)icon[i].kind, num);

				item[i].f_exist = false;
				item[i].x = -100;
				item[i].y = -100;
			}
		}
	}
}