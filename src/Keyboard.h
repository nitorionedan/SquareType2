#pragma once

// 1P Key.
#define DOWN   (0xD0) // 移動
#define UP     (0xC8)
#define RIGHT  (0xCD) // 方向転換
#define LEFT   (0xCB)
#define RSHIFT (0x36) // ショット


// 2P Key.
#define KW     (0x11) // 移動
#define KS     (0x1F)
#define KD     (0x20) // 方向転換
#define KA     (0x1E)
#define SPACE (0x39) // ショット

#define ESC   (0x01)
#define ENTER (0x1C)


// キーの入力状態を更新する
void Keyboard_Update();

// 引数のキーコードのキーの入力状態を返す
int Keyboard_Get(int KeyCode);