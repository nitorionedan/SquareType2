#pragma once

#include <vector>

using namespace std;

class Sound{
public:
	vector<int> vh_se;
	vector<int> vh_se_shot;   // �V���b�g�֌W
	vector<int> vh_se_effect; // �G�t�F�N�g�֌W
	vector<int> vh_music;
};

class Sound_Func{
public:
	void Initialize();
	void Finalize();
	void SE(const int num);
	void SE_NORMAL(const int num);
	void SE_shot(const int num);
	void SE_effect(const int num);
	void Play_Music(const int num);
	void Stop_Music();
};