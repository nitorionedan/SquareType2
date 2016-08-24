#pragma once

#include <vector>

using namespace std;

// ‰æ‘œƒXƒNƒVƒ‡
class Graphic{
public:
	void Initialize();
	void InitGraph();
	void Finalize();
	void Update();
	void Draw();
};

class StatusBoard{
public:
	int x, y;
	vector<int>vh_board;
};