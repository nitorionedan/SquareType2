#include "DxLib.h"
#include "Graphic.h"

using namespace std;

const double PI = 3.14159265358979323846;/* �O���[�o���ϐ� */


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
�������@�摜��\������̈�̒��S���W�i�w�j
�������@�摜��\������̈�̒��S���W�i�x�j
��O�����@�\������摜�̃O���t�B�b�N�n���h��
��l�����@���X�^�[�X�N���[���̎���
��܈����@���X�^�[�X�N���[���̐U����
*/
void Raster_Scroll(const int &X, const int &Y, const int &Gr_Handle, double Cycle, double Shake)
{
	int Width = 0, Height = 0;/*�摜�̉����Əc��*/
	static int Correction = 0;/*���X�^�[�X�N���[���̕␳*/
	GetGraphSize(Gr_Handle, &Width, &Height);
	for (int I = 0; I < Height; ++I) {
		DrawRectGraph(X - Width / 2 + cos((I + Correction) / 180.0 *PI *Cycle) *Shake, Y - Height / 2 + I,
			0, I, Width, 1, Gr_Handle, TRUE, FALSE);
	}
	++Correction;
	return;
}