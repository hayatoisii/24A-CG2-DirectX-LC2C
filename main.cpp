#include <Windows.h>
#pragma warning(push)
//C4023�̌x�������Ȃ��������Ƃɂ���
#pragma warning(disable:4023)
#pragma warning(pop)

// Windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�o�̓E�B���h�E�ւ̕����o��
	OutputDebugStringA("Hello,DirectX!\n");

	return 0;
}