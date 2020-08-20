#include "character.h"
#include "magic.h"
#include <stdio.h>


// キャラクターのマスターデータ
CHARACTER gCharaMaster[] = {

	{
		0, // ID
		"勇者",
		1,		// LV
		50,		// HP
		15,		// MP
		50,		// MAX HP
		15,		// MAX MP
		0,		// EXP

		{ 1 }  // 使える魔法リスト
	},
	{
		1, // ID
		"戦士",
		1,		// LV
		80,		// HP
		0,		// MP
		80,		// MAX HP
		0,		// MAX MP
		0,		// EXP

		{ 0 }  // 使える魔法リスト
	}
};


// レベルアップマスター
CHARACTER_LVUP gCharLvupMaster[] = {
	{	// 勇者のレベルアップテーブル
		{
			{ 0 },  // Lv.0
			{ 10, 50, 15, 1 }, // Lv.1
			{ 20, 52, 16, 0 }, // Lv.2
			{ 30, 54, 17, 2 }, // Lv.3
			{ 40, 56, 18, 0 }, // Lv.4
			{ 50, 58, 20, 0 }, // Lv.5
		}
	},

	{	// 戦士のレベルアップテーブル
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

	if (pCh->exp > gCharLvupMaster[pCh->id].lv[nextLv].exp) {  // 次のLVに必要なEXPを持っているか？
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
	for (int i = 0; i < MAX_MAGIC_LIST; i++) {  // 対象キャラの習得済み魔法リストを調査
		if (pCh->magicIDList[i] == magicID) {  // 覚えようとしている魔法がすでに習得済み
			break;
		}
		else if (pCh->magicIDList[i] == 0) {  // 空きスロット発見
			pCh->magicIDList[i] = magicID;  // 覚える
			break;
		}
	}
}


void Character_PrintStatus(CHARACTER * pCh)
{
	printf("%s\n", pCh->name);

	printf("LV: %d\n", pCh->lv);

	// HPが少なかったら表示色を変える
	if (pCh->hp < pCh->max_hp / 3) {  // HPが1/3未満なら
		if (pCh->hp < pCh->max_hp / 5) {  // HPが1/5未満なら
			printf("\x1b[31m");  // 文字色を赤に変えるコマンド
		}
		else {
			printf("\x1b[33m");  // 文字色を黄色に変えるコマンド
		}
	}

	printf("HP: %d / %d\n", pCh->hp, pCh->max_hp);

	printf("\x1b[39m");  // 文字色を元に戻すコマンド

	printf("MP: %d / %d\n", pCh->mp, pCh->max_mp);
}


