/*�@���ׂẲ����܂Ƃ߂�Ƃ����n�b�L�������ăA�z�Ȕ��z�ł��B���̂Ƃ��̎��������肽���B�@*/

#include "DxLib.h"
#include "Sound.h"


static class Sound sound;
static class Sound_Func soundf;


enum Stage_Music{
	Inst,
	Menu,
	SelectType,
	SelectStage,
	FirstStage,
	SecondStage
};


void Sound_Func::Initialize()
{
	// System SE
	sound.vh_se.push_back(LoadSoundMem("SOUND/SE/selectblip.wav"));
	sound.vh_se.push_back(LoadSoundMem("SOUND/SE/ch_select.mp3"));
	sound.vh_se.push_back(LoadSoundMem("SOUND/SE/menukettei.mp3"));

	// Shot
	sound.vh_se_shot.push_back(LoadSoundMem("SOUND/SE/shot03_2.mp3"));

	// Effect
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/kin.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/damage00.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/speedup.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/accelup.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/blockhit.wav")); // 4

	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/rtup.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/recovery.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/hpup.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/powup.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/dokuro.wav")); // 9

	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/eshot.wav"));
	sound.vh_se_effect.push_back(LoadSoundMem("SOUND/SE/breakblock.wav"));

	// Music 5����
	sound.vh_music.push_back(LoadSoundMem("SOUND/tutorial.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/title.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/forest.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/ruin.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/ice.mp3")); // 4

	sound.vh_music.push_back(LoadSoundMem("SOUND/fire.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/sea.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/mystery.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/space.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/fire.mp3")); // 9

	sound.vh_music.push_back(LoadSoundMem("SOUND/StageSelect.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/result.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/machine.mp3"));
	sound.vh_music.push_back(LoadSoundMem("SOUND/ch_select.mp3"));
}


void Sound_Func::Finalize()
{
	for (auto itr = sound.vh_music.begin(); itr < sound.vh_music.end(); ++itr)			DeleteSoundMem(*itr);
	for (auto itr = sound.vh_se_effect.begin(); itr < sound.vh_se_effect.end(); ++itr)	DeleteSoundMem(*itr);
	for (auto itr = sound.vh_se_shot.begin(); itr < sound.vh_se_shot.end(); ++itr)		DeleteSoundMem(*itr);
	sound.vh_music.clear();
	sound.vh_se_effect.clear();
	sound.vh_se_shot.clear();
}


void Sound_Func::SE(const int num)
{
	PlaySoundMem(sound.vh_se[num], DX_PLAYTYPE_BACK);
	if (PlaySoundMem(sound.vh_se[num], DX_PLAYTYPE_BACK) == -1)	printfDx("Sound is failed! [se]");
}


void Sound_Func::SE_NORMAL(const int num)
{
	PlaySoundMem(sound.vh_se[num], DX_PLAYTYPE_NORMAL);
	if (PlaySoundMem(sound.vh_se[num], DX_PLAYTYPE_NORMAL) == -1)	printfDx("Sound is failed! [se]");
}


/*
�@�@�\�F���ʉ��Đ�
�@�����F0(red), 1(blue), 2(yellow)...
*/
void Sound_Func::SE_shot(const int num)
{
	PlaySoundMem(sound.vh_se_shot[num], DX_PLAYTYPE_BACK);
	if (PlaySoundMem(sound.vh_se_shot[num], DX_PLAYTYPE_BACK) == -1)	printfDx("Sound is failed! [shot]");
}


/*
�@�@�\�F���ʉ��Đ�
�@�����F0
*/
void Sound_Func::SE_effect(const int num)
{
	for (auto itr = sound.vh_se_effect.begin(); itr < sound.vh_se_effect.end(); itr++)
	{
		if (*itr == num) 	PlaySoundMem(sound.vh_se_effect[num], DX_PLAYTYPE_BACK);
	}

//	printfDx("%d", sound.vh_se_effect.capacity());
	if (PlaySoundMem(sound.vh_se_effect[num], DX_PLAYTYPE_BACK) == -1)	printfDx("Sound is failed! [effect]");
}


/*
�@�@�\�F���y�Đ�
�@�����F0.Inst 1.Menu 2.SelectType 3.SelectStage 4.FirstStage ...
*/
void Sound_Func::Play_Music(const int num)
{
	ChangeVolumeSoundMem(150, sound.vh_music[num]);
	PlaySoundMem(sound.vh_music[num], DX_PLAYTYPE_LOOP);
	if (PlaySoundMem(sound.vh_music[num], DX_PLAYTYPE_LOOP) == -1)	printfDx("Sound is failed! [music]");
}


/*
�@�@�\�F���y��~
�@�����F�Ȃ�
*/
void Sound_Func::Stop_Music()
{
	for (auto itr = sound.vh_music.begin(); itr < sound.vh_music.end(); ++itr)
	{
		if (CheckSoundMem(*itr) == 1) // �Đ����̃t�@�C��������Ύ~�߂�
		{
			StopSoundMem(*itr);
			break;
		}
	}
}