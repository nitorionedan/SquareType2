#pragma once

// 1P Key.
#define DOWN   (0xD0) // �ړ�
#define UP     (0xC8)
#define RIGHT  (0xCD) // �����]��
#define LEFT   (0xCB)
#define RSHIFT (0x36) // �V���b�g


// 2P Key.
#define KW     (0x11) // �ړ�
#define KS     (0x1F)
#define KD     (0x20) // �����]��
#define KA     (0x1E)
#define SPACE (0x39) // �V���b�g

#define ESC   (0x01)
#define ENTER (0x1C)


// �L�[�̓��͏�Ԃ��X�V����
void Keyboard_Update();

// �����̃L�[�R�[�h�̃L�[�̓��͏�Ԃ�Ԃ�
int Keyboard_Get(int KeyCode);