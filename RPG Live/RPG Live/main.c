#include <stdio.h>
#include <stdlib.h>  // <>�̎��́AVS�ɕt������W���w�b�_�[�t�@�C��
#include <time.h>
#include <string.h>

#include "player.h"  // ����w�b�_�[���C���N���[�h����""���g��
#include "character.h"
#include "magic.h"
#include "item.h"


// Game Status�̒萔��`
enum GAMESTATUS {
	GS_APP_END,
	GS_TITLE,  // �^�C�g�����
	GS_OPENING,  // �I�[�v�j���O���
	GS_TOWN,  // �����
	GS_SHOP,  // ���X���
	GS_SHOP_BUY,  // ���X�̔������
	GS_SHOP_SELL,  // ���X�̔�����

	GS_DUNGEON,  // �_���W�������
	GS_BATTLE, // �퓬���
	GS_TREASURE, // �󔠉��
	GS_STAIRS, // �K�i�݂�������

	GS_GAMEOVER, // �Q�[���I�[�o�[���

	GS_STATUS, // �X�e�[�^�X���
	GS_USE_ITEM, // �A�C�e���g�p���
	GS_USE_MAGIC, // ���@�g�p���

	GS_LOADING, // ���[�f�B���O���
};



// �֐��̃v���g�^�C�v�錾
void ShowTownBilboard(void);
void ShowShopBilboard(int);
void ShowDungeonBillboard(void);
void WaitForEnter(void);
void ShowMessage(int);
void PrintAlignedStr(const char* str, int maxlen);

int Get1of3(int, int, int);
int Get1of4(int, int, int, int);

void save(void);
void load(void);

void GsTitle(void);
void GsOpening(void);
void GsTown(void);
void GsShop(void);
void GsShopBuy(void);
void GsShopSell(void);
void GsDungeon(void);
void GsBattle(void);
void GsTreasure(void);
void GsStairs(void);
void GsGameover(void);
void GsStatus(void);
void GsUseItem(void);
void GsUseMagic(void);
void GsLoading(void);


// �O���[�o���ϐ�

// ���b�Z�[�W�E�}�X�^�[�f�[�^
char* gMsgMaster[] = {
	"�퓬�G���J�E���g�I",
	"�󔠂��������I",
	"�K�i���������I",
	"�Q�[���I�[�o�[�I�I"
};



// �󔠂͏�ɂR���ŃA�C�e�������������Ă���d�l
typedef struct {
	ITEM* item[3];
} TREASURE_ITEM;

// �󔠃A�C�e���}�X�^�[
TREASURE_ITEM gTreasureItemMaster[] = {
	{ NULL },  // �O�K
	{ &gItemMaster[0], &gItemMaster[3], &gItemMaster[6]  },  // �P�K
	{ &gItemMaster[0], &gItemMaster[3], &gItemMaster[8] },  // �Q�K
	{ &gItemMaster[0], &gItemMaster[3], &gItemMaster[5] },  // �R�K
	{ &gItemMaster[0], &gItemMaster[4], &gItemMaster[7] },  // �S�K
};



int gGameStatus = GS_TOWN;  // ���݂̉�ʂ�\���ԍ�������ϐ��@�@���Z�[�u�Ώ�


PLAYER gPlayer;  // player.h�Œ�`����PLAYER�\���̌^�̕ϐ���錾�@�@���Z�[�u�Ώ�

#define MAX_PARTY_MEMBER  2
CHARACTER gParty[MAX_PARTY_MEMBER]; // �p�[�e�B�[�S���̃p�����[�^�����z��@�@���Z�[�u�Ώ�

unsigned int gStartTime;


// ���C���֐�
void main(void) {

	// �C���x���g���z������ׂĂO�ɂ���
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		gPlayer.inventory[i] = 0;
	}

	gPlayer.gold = 500;

	gParty[0] = gCharaMaster[0];  // �E�҂�LV1��Ԃ̃p�����[�^���R�s�[
	gParty[1] = gCharaMaster[1];  // ��m��LV1��Ԃ̃p�����[�^���R�s�[

	srand((unsigned int)time(NULL));  // �������X�g�̏������E����

	load();

	while (gGameStatus != GS_APP_END) {  // �Q�[�����[�v

		switch (gGameStatus) {  // ��ʐ؂�ւ��̂��߂�switch��

		case GS_TITLE:  // �^�C�g����ʂ̏���
			GsTitle();
			break;

		case GS_OPENING:  // �I�[�v�j���O�̏���
			GsOpening();
			break;

		case GS_TOWN:  // ���̏���
			GsTown();
			break;

		case GS_SHOP:  // ���X�̏���
			GsShop();
			break;

		case GS_SHOP_BUY: // ���X�@�A�C�e������
			GsShopBuy();
			break;

		case GS_SHOP_SELL: // ���X�@�A�C�e������
			GsShopSell();
			break;

		case GS_DUNGEON: // �_���W����
			GsDungeon();
			break;

		case GS_BATTLE: // �퓬
			GsBattle();
			break;

		case GS_TREASURE: // ��
			GsTreasure();
			break;

		case GS_STAIRS: // �K�i�݂������
			GsStairs();
			break;

		case GS_GAMEOVER:
			GsGameover();
			break;

		case GS_STATUS:
			GsStatus();
			break;

		case GS_USE_ITEM:
			GsUseItem();
			break;

		case GS_USE_MAGIC:
			GsUseMagic();
			break;

		case GS_LOADING:
			GsLoading();
			break;
		}

	}
}



void GsTitle(void) {
	system("cls");
	printf("TALES OF DRAGON FANTASY\n\n");
	WaitForEnter();
	gGameStatus = GS_OPENING;
}


void GsOpening(void) {
	system("cls");

	// ���@���O���͏���
	printf("���O����͂��Ă��������B >>> ");
	scanf_s("%s", gPlayer.myName, 32);
	printf("�����I%s��I�悭���Q�����I\n", gPlayer.myName);
	// ��  ���O���͏���

	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("�I�[�v�j���O��ʂł�\n");
	WaitForEnter();
	gGameStatus = GS_TOWN;
}


void GsTown(void) {
	int command;

	// ���ɖ߂�����HP��MP�S��
	Character_RecoverHpMax(&gParty[0]);
	Character_RecoverMpMax(&gParty[0]);
	Character_RecoverHpMax(&gParty[1]);
	Character_RecoverMpMax(&gParty[1]);

	system("cls");

	// �҂��̊Ŕ\��
	ShowTownBilboard();

	printf("�R�}���h�ꗗ\n");
	printf("1. ���X�ɍs��\n");
	printf("2. �_���W�����ɍs��\n");
	printf("3. �X�e�[�^�X���\n");
	printf("0. �Q�[���I��\n");

	printf(">>> �R�}���h���� >>>");
	scanf_s("%d", &command);

	if (command == 1) {  // ���X�ɍs�����I�΂ꂽ��
		gGameStatus = GS_SHOP;
	}
	else if (command == 2) {  // �_���W�����ɍs�����I�΂ꂽ��
	//	gGameStatus = GS_DUNGEON;
		gGameStatus = GS_LOADING;
		gStartTime = (unsigned int)time(NULL);
		gPlayer.floorNo = 1;  // �t���A�ԍ����P�K�ɃZ�b�g
	}
	else if (command == 3) {  // �X�e�[�^�X��ʂɍs�����I�΂ꂽ��
		gGameStatus = GS_STATUS;
	}
	else if (command == 0) {  // �Q�[���I�����I�΂ꂽ��
		gGameStatus = GS_APP_END;
	}
}


void GsShop(void) {
	int command;

	system("cls");

	// ���X�̊Ŕ\���i�����S�[���h���\���j
	ShowShopBilboard(gPlayer.gold);

	printf("1. ����\n");
	printf("2. ����\n");
	printf("3. ��߂�\n");

	printf("�R�}���h���� >>>");
	scanf_s("%d", &command);

	if (command == 1) {
		gGameStatus = GS_SHOP_BUY;
	}
	else if (command == 2) {
		gGameStatus = GS_SHOP_SELL;
	}
	else if (command == 3) {
		gGameStatus = GS_TOWN;
	}
}


void GsShopBuy(void) {
	int command;

	system("cls");
	// ���X�̊Ŕ\���i�����S�[���h���\���j
	ShowShopBilboard(gPlayer.gold);
	printf("���i�ꗗ\n");

	// ���i���X�g�\��
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		// �ԍ��@�A�C�e�����@�l�i�@��\��
		printf("%d %s   %d GOLD\n", i + 1, gItemMaster[i].name, gItemMaster[i].price);
	}

	printf("99. ���ǂ�\n");

	printf("�ǂ���w�����܂����H�@�ԍ��Ŏw�� >>>");
	scanf_s("%d", &command);

	if (command == 99) {  // ���ǂ��I�񂾎�
		gGameStatus = GS_SHOP;
	}
	else if (command <= GetNumberOfItemMaster()) {  // �����Ă�A�C�e�����傫�Ȑ��������͂���Ă���͂���
		gPlayer.gold -= gItemMaster[command - 1].price;  // �w�����ꂽ�A�C�e���̉��i���A�����������炷
		gPlayer.inventory[command - 1]++;  // �C���x���g���̏����A�C�e�������{�P����
	}
}


void GsShopSell(void) {
	int command;

	system("cls");
	// ���X�̊Ŕ\���i�����S�[���h���\���j
	ShowShopBilboard(gPlayer.gold);
	printf("�ǂ̃A�C�e���𔄂�܂����H\n");

	// ����A�C�e���̃��X�g��\��
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		// �C���x���g���̌����P�ȏ�̃A�C�e�������\��
		if (gPlayer.inventory[i] > 0) {
			// �ԍ��@�A�C�e�����@�l�i�@��\��
			printf("%d %s   %d GOLD\n", i + 1, gItemMaster[i].name, gItemMaster[i].price / 2);
		}
	}

	printf("99. ���ǂ�\n");

	printf("�ǂ�𔄂�܂����H�@�ԍ��Ŏw�� >>>");
	scanf_s("%d", &command);

	if (command == 99) {  // ���ǂ��I�񂾎�
		gGameStatus = GS_SHOP;
	}
	else if (command <= GetNumberOfItemMaster()) {  // �A�C�e���}�X�^�[�ő�l���傫�Ȑ��������͂��ꂽ��͂���
		if (gPlayer.inventory[command - 1] > 0) {  // �C���x���g���O�̃A�C�e�����I�����ꂽ��͂���
			gPlayer.inventory[command - 1]--;  // �C���x���g���̊Y���A�C�e���̌����P���炷
			gPlayer.gold += gItemMaster[command - 1].price / 2;  // �̔����i��1/2�̃S�[���h�𑝂₷
		}
	}
}


void GsDungeon(void) {
	int command;  // ANSI-C�̏ꍇ�͊֐��̈�ԏ�ɕϐ��錾������

	system("cls");
	ShowDungeonBillboard();

	printf("1. ���ɐi��\n");
	printf("2. �A�C�e�����g��\n");
	printf("3. ���@���g��\n");
	printf("0. ���ɖ߂�\n");

	printf("�R�}���h���� >>>");

	scanf_s("%d", &command);

	if (command == 1) {
		// �R���Łu�퓬�E�󔠁E�K�i�v������
		int result = Get1of3(50, 45, 5);

		switch (result) {
		case 1:  // �퓬
			gGameStatus = GS_BATTLE;
			break;
		case 2:  // �󔠌�����
			gGameStatus = GS_TREASURE;
			break;
		case 3:  // �K�i
			gGameStatus = GS_STAIRS;
			break;
		}
	}
	else if (command == 2) {
		gGameStatus = GS_USE_ITEM;
	}
	else if (command == 3) {
		gGameStatus = GS_USE_MAGIC;
	}
	else if (command == 0) {
		gGameStatus = GS_TOWN;
	}
}


void GsBattle(void) {
	system("cls");
	ShowMessage(0);

	gGameStatus = GS_DUNGEON;

	// �^�[�Q�b�g���߂鏈��
	for (;;) {  // �i�v���[�v
		CHARACTER* pTarget = &gParty[rand() % 2]; // �^�[�Q�b�g�̃L�����f�[�^�̃A�h���X�擾

		if (pTarget->hp > 0) {  // �^�[�Q�b�g�������Ă邩�ǂ���
			pTarget->hp -= rand() % (pTarget->max_hp / 4); // �����_����MAXHP��1/4�̃_���[�W

			if (pTarget->hp < 0) {  // �I�[�o�[�L�����ꂽ�Ƃ���HP���O�ɖ߂�
				pTarget->hp = 0;
			}

			break;  // ���[�v�𑦍��ɔ�����
		}
	}

	// �Q�[���I�[�o�[����
	if (gParty[0].hp <= 0) {  // HP���O�ȉ��ɂȂ�����
		if (gParty[1].hp <= 0) {  // HP���O�ȉ��ɂȂ�����
			gGameStatus = GS_GAMEOVER;  // �Q�[���I�[�o�[��ʂɍs��
			return;  // �Q�[���I�[�o�[�Ȃ炱��ȍ~�̏��������s���Ȃ�
		}
	}

	// EXP�l������
	int exp = rand() % 10 + 2;
	printf("EXP��%d�l�������I\n", exp);

	if (gParty[0].hp > 0) {
		Character_GetExp(&gParty[0], exp);
	}
	if (gParty[0].hp > 1) {
		Character_GetExp(&gParty[1], exp);
	}

	WaitForEnter();
}


void GsTreasure(void) {
	system("cls");
	ShowMessage(1);

	ITEM* getItem;
	int result;

	result = Get1of3(60, 30, 10) - 1;  // �R�����烉���_���łP����������Ă���

	getItem = gTreasureItemMaster[gPlayer.floorNo].item[result];  // ��ɓ��ꂽ�A�C�e���̃}�X�^�[�f�[�^�̃A�h���X���擾

	printf("\n\n");
	printf("�󔠂���u%s�v����ɓ��ꂽ�I\n", getItem->name);

	gPlayer.inventory[getItem->itemNo]++;  // �v���C���[�̃C���x���g���̊Y���A�C�e���̌����{�P

	WaitForEnter();
	gGameStatus = GS_DUNGEON;
}


void GsStairs(void) {
	system("cls");
	ShowMessage(2);
	WaitForEnter();
	gGameStatus = GS_DUNGEON;
	gPlayer.floorNo++;  // �K�i����������A���݂̊K�w���{�P
}


void GsGameover(void)
{
	system("cls");
	ShowMessage(3);
	WaitForEnter();
	gGameStatus = GS_TITLE; // �Q�[���I�[�o�[��ʂ���̓^�C�g����ʂɂ����s���Ȃ�
}


void PrintNumber(int number) {
	if (number < 10) {
		printf(" ");
	}
	if (number < 100) {
		printf(" ");
	}
	if (number < 1000) {
		printf(" ");
	}

	printf("%d", number);
}


void GsStatus(void)
{
	system("cls");

	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("�X�e�[�^�X \n");
	printf("��������������������������������\n");

	printf("\n\n");
	printf("����GOLD  %d\n", gPlayer.gold);
	printf("\n\n");

	for (int i = 0; i < MAX_PARTY_MEMBER; i++) {
		printf("%s\n", gParty[i].name);
		printf("LV: %d\n", gParty[i].lv);
		printf("HP: %d  /  %d\n", gParty[i].hp, gParty[i].max_hp);

		printf("MP: ");
		PrintNumber(gParty[i].mp);
		printf("  /  ");
		PrintNumber(gParty[i].max_mp);
		printf("\n");
		//printf("MP: %d  /  %d\n", gParty[i].mp, gParty[i].max_mp);

		//printf("������: %d\n", gParty[i].strength);

		printf("\n----------\n");
	}

	WaitForEnter();
	gGameStatus = GS_TOWN;
}

void GsUseItem(void)
{
	int command;

	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("�A�C�e���g�p��� \n");
	printf("��������������������������������\n");

	printf("\n");

	// �g���A�C�e���̃��X�g��\��
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		// �C���x���g���̌����P�ȏ�̃A�C�e�������\��
		if (gPlayer.inventory[i] > 0) {
			// �ԍ��@�A�C�e�����@�������@��\��
			printf("%d %s x %d\n", i + 1, gItemMaster[i].name, gPlayer.inventory[i]);
		}
	}

	printf("0. ���ǂ�\n");

	printf("�ǂ���g���܂����H�@�ԍ��Ŏw�� >>>");
	scanf_s("%d", &command);

	if (command == 0) {  // ���ǂ��I�񂾎�
		gGameStatus = GS_DUNGEON;
	}
	else if (command <= GetNumberOfItemMaster()) {  // �A�C�e���}�X�^�[�ő�l���傫�Ȑ��������͂��ꂽ��͂���
		if (gPlayer.inventory[command - 1] > 0) {  // �C���x���g���O�̃A�C�e�����I�����ꂽ��͂���
			if (gItemMaster[command - 1].type == ITYPE_HPRECOVER) {
				gPlayer.inventory[command - 1]--;  // �C���x���g���̊Y���A�C�e���̌����P���炷

				// �A�C�e���̌��ʂ𔭌�������
				printf("�N�Ɏg���܂����H [0.%s 1.%s] >>>", gParty[0].name, gParty[1].name);
				int charNo;
				scanf_s("%d", &charNo);

				gParty[charNo].hp += gItemMaster[command - 1].value;

				if (gParty[charNo].hp > gParty[charNo].max_hp) {
					gParty[charNo].hp = gParty[charNo].max_hp;
				}
			}
			else if (gItemMaster[command - 1].type == ITYPE_MPRECOVER) {
				gPlayer.inventory[command - 1]--;  // �C���x���g���̊Y���A�C�e���̌����P���炷
			}
			else {

			}



		}
	}
}


void GsUseMagic(void)
{
	int command;
	MAGIC* pMagicUse;  // �g�����@�̃}�X�^�[�f�[�^�̃A�h���X������ϐ�
	CHARACTER* pCharOwner;  // ���@�g���L�����̃A�h���X������ϐ�
	CHARACTER* pCharTarget;  // ���@���g���ΏۃL�����̃A�h���X������ϐ�

	system("cls");
	printf("\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("���@�g�p��� \n");
	printf("��������������������������������\n");

	printf("\n");

	printf("�ǂ̃L�����̖��@���g���܂����H [1.%s 2.%s 0.���ǂ�] >>>", gParty[0].name, gParty[1].name);
	scanf_s("%d", &command);

	if (command == 0) {
		gGameStatus = GS_DUNGEON;
		return;
	}
	else if (command > 2) { // �G���[�`�F�b�N
		return;
	}

	pCharOwner = &gParty[command - 1];  // ���@�g�p�L�����̃A�h���X���Z�b�g

	// �g�����@�̃��X�g��\��
	for (int i = 0; i < MAX_MAGIC_LIST; i++) {
		// ���łɏK�����Ă��閂�@�����\��
		if (pCharOwner->magicIDList[i] != 0) {  // 0�͖��@�X���b�g����
			MAGIC* pMagic = GetMagic(pCharOwner->magicIDList[i]);
			// �ԍ��@�A�C�e�����@�������@��\��
			printf("%d. ", i + 1);
			PrintAlignedStr(pMagic->name, 12);
			printf("MP %d\n", pMagic->mp);
		}
	}

	printf("0. ���ǂ�\n");

	printf("�ǂ���g���܂����H�@�ԍ��Ŏw�� >>>");
	scanf_s("%d", &command);

	pMagicUse = GetMagic(pCharOwner->magicIDList[command - 1]); // �g�����@�̃A�h���X���Z�b�g

	if (command == 0) {  // ���ǂ��I�񂾎�
		return;
	}
	else if (command <= MAX_MAGIC_LIST) {
		if (pMagicUse->type == MTYPE_RECOVER) {  // �񕜖��@�͎g����
			printf("�N�Ɏg���܂����H [1.%s 2.%s] >>>", gParty[0].name, gParty[1].name);
			scanf_s("%d", &command);

			pCharTarget = &gParty[command - 1];  // �g�p�ΏۃL�����̃A�h���X���Z�b�g

			if (pCharOwner->mp >= pMagicUse->mp) {  // MP����邩�`�F�b�N
				Magic_Use(pMagicUse, pCharOwner, pCharTarget);
			}
			else {
				printf("MP������Ȃ��I\n");
				WaitForEnter();
			}
		}
		else {
			printf("���̖��@�͂����ł͎g���Ȃ��I\n");
			WaitForEnter();
		}
	}
}


void GsLoading(void)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("Now Loading ... \n");
	printf("��������������������������������\n");

	if (time(NULL) > gStartTime + 1) {  // ���[�f�B���O��ʂɗ����u�Ԃ̎��Ԃ���A�P�b�o������
		gGameStatus = GS_DUNGEON;
		// �I�[�g�Z�[�u�̏ꍇ�́A������save�֐����Ăяo���Ɨǂ�
		save();
	}
}


void ShowMessage(int msgNo) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf(gMsgMaster[msgNo]);
	printf("\n");
	printf("��������������������������������\n");
}


void PrintAlignedStr(const char * str, int maxlen)
{
	printf(str);

	for (int i = 0; i < maxlen - (int)strlen(str); i++) {
		putc(' ', stdout);
	}
}



void ShowTownBilboard(void) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("�����͒��ł� \n");
	printf("��������������������������������\n");
}


void ShowShopBilboard(int myGold) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("�����͂��X�ł�    �������F%d GOLD\n", myGold);
	printf("��������������������������������\n");
}


void ShowDungeonBillboard(void) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // �e��ʌ��܂����ʒu�Ƀ��b�Z�[�W���o�����߂̉��s
	printf("��������������������������������\n");
	printf("�_���W����    %d �K\n", gPlayer.floorNo);
	printf("��������������������������������\n");

	printf("\n\n");

	printf("�X�e�[�^�X\n");

	Character_PrintStatus(&gParty[0]);
	printf("\n\n");
	Character_PrintStatus(&gParty[1]);

	printf("\n\n");
}


void WaitForEnter(void) {
	printf("PRESS ENTER KEY...");
	rewind(stdin);
	getchar();
}



// �R������P���w�肵���m���őI�Ԋ֐�
// �����F
// int per1�F�I�����P�̒��I�m��
// int per2�F�I�����Q�̒��I�m��
// int per3�F�I�����R�̒��I�m��
// �߂�l�F
// ���I���ʂ𐮐��ŕԂ��i�P�`�R�j
int Get1of3(int per1, int per2, int per3) {
	return Get1of4(per1, per2, per3, 0);
}


// �S������P���w�肵���m���őI�Ԋ֐�
// �����F
// int per1�F�I�����P�̒��I�m��
// int per2�F�I�����Q�̒��I�m��
// int per3�F�I�����R�̒��I�m��
// int per4�F�I�����S�̒��I�m��
// �߂�l�F
// ���I���ʂ𐮐��ŕԂ��i�P�`�S�j
int Get1of4(int per1, int per2, int per3, int per4) {
	int number = rand() % (per1 + per2 + per3 + per4);  // �S�̊m���̍��v�l�����̗���������Ă���

	if (number < per1) {  // �m��per1�͈̔͂Ȃ�
		return 1;
	}
	else if (number < per1 + per2) {  // �m��per2�͈̔͂Ȃ�
		return 2;
	}
	else if (number < per1 + per2 + per3) {  // �m��per3�͈̔͂Ȃ�
		return 3;
	}
	else {  //����ȊO�iper1�ł�per2�ł��Ȃ��͈́j�Ȃ�
		return 4;
	}
}

void save(void)
{
	FILE* fp;
	errno_t isOpen = fopen_s(&fp, "save.dat", "wb");  // �o�C�i�����[�h

	if (isOpen == 0) {  // �t�@�C���I�[�v��������������

		// �f�[�^�������ݏ���
		// gGameStatus�̏�������
		fwrite(&gGameStatus, sizeof(gGameStatus), 1, fp);

		// gPlayer�̏�������
		fwrite(&gPlayer, sizeof(gPlayer), 1, fp);

		// gParty�̏�������
		fwrite(&gParty[0], sizeof(CHARACTER), 2, fp);

		fclose(fp);

		printf("�I�[�g�Z�[�u�����I");
	}
	else {
		printf("�I�[�g�Z�[�u���s�I");
	}
}

void load(void)
{
	FILE* fp;
	errno_t isOpen = fopen_s(&fp, "save.dat", "rb");  // �o�C�i�����[�h

	if (isOpen == 0) {  // �t�@�C���I�[�v��������������

		// �f�[�^�������ݏ���
		// gGameStatus�̓ǂݍ���
		fread(&gGameStatus, sizeof(gGameStatus), 1, fp);

		// gPlayer�̏�������
		fread(&gPlayer, sizeof(gPlayer), 1, fp);

		// gParty�̏�������
		fread(&gParty[0], sizeof(CHARACTER), 2, fp);

		fclose(fp);

		printf("�I�[�g���[�h�����I");
	}
	else {
		printf("�I�[�g���[�h���s�I");
	}
}
