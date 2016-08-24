#pragma once

#include <vector>

using namespace std;

class Sound{
public:
	vector<int> vh_se;
	vector<int> vh_se_shot;   // ショット関係
	vector<int> vh_se_effect; // エフェクト関係
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