#include "item.h"


// �A�C�e���}�X�^�[�̔z��
ITEM gItemMaster[] = {
	{ ITYPE_HPRECOVER, 0, "��", 10, 8 },			// 0
	{ ITYPE_MPRECOVER, 1, "���@�̏��r", 10, 50 },
	{ ITYPE_WEAPON, 2, "�͂��˂̌�", 80, 300 },
	{ ITYPE_ETC, 3, "��������", 10, 30 },
	{ ITYPE_ETC, 4, "����������̗t", 10, 1000 },
	{ ITYPE_ETC, 5, "������̎�", 10, 500 },			// 5
	{ ITYPE_ETC, 6, "�ǂ���������", 10, 15 },
	{ ITYPE_WEAPON, 7, "�h���S���o�X�^�[", 10, 30000 },
	{ ITYPE_WEAPON, 8, "�ǂ��̑匕", 10, 30 },			// 8
};


int GetNumberOfItemMaster(void)
{
	return sizeof(gItemMaster) / sizeof(ITEM);
}
