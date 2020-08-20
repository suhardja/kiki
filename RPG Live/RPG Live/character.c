#include "character.h"
#include "magic.h"
#include <stdio.h>


// �L�����N�^�[�̃}�X�^�[�f�[�^
CHARACTER gCharaMaster[] = {

	{
		0, // ID
		"�E��",
		1,		// LV
		50,		// HP
		15,		// MP
		50,		// MAX HP
		15,		// MAX MP
		0,		// EXP

		{ 1 }  // �g���閂�@���X�g
	},
	{
		1, // ID
		"��m",
		1,		// LV
		80,		// HP
		0,		// MP
		80,		// MAX HP
		0,		// MAX MP
		0,		// EXP

		{ 0 }  // �g���閂�@���X�g
	}
};


// ���x���A�b�v�}�X�^�[
CHARACTER_LVUP gCharLvupMaster[] = {
	{	// �E�҂̃��x���A�b�v�e�[�u��
		{
			{ 0 },  // Lv.0
			{ 10, 50, 15, 1 }, // Lv.1
			{ 20, 52, 16, 0 }, // Lv.2
			{ 30, 54, 17, 2 }, // Lv.3
			{ 40, 56, 18, 0 }, // Lv.4
			{ 50, 58, 20, 0 }, // Lv.5
		}
	},

	{	// ��m�̃��x���A�b�v�e�[�u��
		{
			{ 0 },  // Lv.0
			{ 10, 80, 0, 0 }, // Lv.1
			{ 20, 82, 0, 0 }, // Lv.2
			{ 30, 85, 0, 0 }, // Lv.3
			{ 40, 87, 0, 0 }, // Lv.4
			{ 50, 90, 0, 0 }, // Lv.5
		}
	}
};



void Character_RecoverHpMax(CHARACTER* pCh) {
	pCh->hp = pCh->max_hp;
}


void Character_RecoverMpMax(CHARACTER * pCh)
{
	pCh->mp = pCh->max_mp;
}


void Character_RecoverHp(CHARACTER * pCh, int value)
{
	pCh->hp += value;

	if (pCh->hp > pCh->max_hp) {
		pCh->hp = pCh->max_hp;
	}
}

void Character_ConsumeMp(CHARACTER * pCh, int value)
{
	pCh->mp -= value;

	if (pCh->mp < 0) {
		pCh->mp = 0;
	}
}


void Character_GetExp(CHARACTER * pCh, int exp)
{
	pCh->exp += exp;
	Character_Lvup(pCh);
}


void Character_Lvup(CHARACTER * pCh)
{
	const int nextLv = pCh->lv + 1;

	if (pCh->exp > gCharLvupMaster[pCh->id].lv[nextLv].exp) {  // ����LV�ɕK�v��EXP�������Ă��邩�H
		pCh->lv++;
		pCh->max_hp = gCharLvupMaster[pCh->id].lv[nextLv].hp;
		pCh->max_mp = gCharLvupMaster[pCh->id].lv[nextLv].mp;

		if (gCharLvupMaster[pCh->id].lv[nextLv].magicID != 0) {
			Character_LearnMagic(pCh, gCharLvupMaster[pCh->id].lv[nextLv].magicID);
		}
	}
}

void Character_LearnMagic(CHARACTER * pCh, int magicID)
{
	for (int i = 0; i < MAX_MAGIC_LIST; i++) {  // �ΏۃL�����̏K���ςݖ��@���X�g�𒲍�
		if (pCh->magicIDList[i] == magicID) {  // �o���悤�Ƃ��Ă��閂�@�����łɏK���ς�
			break;
		}
		else if (pCh->magicIDList[i] == 0) {  // �󂫃X���b�g����
			pCh->magicIDList[i] = magicID;  // �o����
			break;
		}
	}
}


void Character_PrintStatus(CHARACTER * pCh)
{
	printf("%s\n", pCh->name);

	printf("LV: %d\n", pCh->lv);

	// HP�����Ȃ�������\���F��ς���
	if (pCh->hp < pCh->max_hp / 3) {  // HP��1/3�����Ȃ�
		if (pCh->hp < pCh->max_hp / 5) {  // HP��1/5�����Ȃ�
			printf("\x1b[31m");  // �����F��Ԃɕς���R�}���h
		}
		else {
			printf("\x1b[33m");  // �����F�����F�ɕς���R�}���h
		}
	}

	printf("HP: %d / %d\n", pCh->hp, pCh->max_hp);

	printf("\x1b[39m");  // �����F�����ɖ߂��R�}���h

	printf("MP: %d / %d\n", pCh->mp, pCh->max_mp);
}


