#include "DxLib.h"
#include "Graphic.h"

using namespace std;

const double PI = 3.14159265358979323846;/* グローバル変数 */


void Graphic::Initialize()
{

}


void Graphic::Finalize()
{

}


void Graphic::InitGraph()
{
}

void Graphic::Draw()
{
}
/*
第一引数　画像を表示する領域の中心座標（Ｘ）
第二引数　画像を表示する領域の中心座標（Ｙ）
第三引数　表示する画像のグラフィックハンドル
第四引数　ラスタースクロールの周期
第五引数　ラスタースクロールの振動幅
*/
void Raster_Scroll(const int &X, const int &Y, const int &Gr_Handle, double Cycle, double Shake)
{
	int Width = 0, Height = 0;/*画像の横幅と縦幅*/
	static int Correction = 0;/*ラスタースクロールの補正*/
	GetGraphSize(Gr_Handle, &Width, &Height);
	for (int I = 0; I < Height; ++I) {
		DrawRectGraph(X - Width / 2 + cos((I + Correction) / 180.0 *PI *Cycle) *Shake, Y - Height / 2 + I,
			0, I, Width, 1, Gr_Handle, TRUE, FALSE);
	}
	++Correction;
	return;
}