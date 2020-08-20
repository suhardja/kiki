#include <stdio.h>
#include <stdlib.h>  // <>の時は、VSに付属する標準ヘッダーファイル
#include <time.h>
#include <string.h>

#include "player.h"  // 自作ヘッダーをインクルード時は""を使う
#include "character.h"
#include "magic.h"
#include "item.h"


// Game Statusの定数定義
enum GAMESTATUS {
	GS_APP_END,
	GS_TITLE,  // タイトル画面
	GS_OPENING,  // オープニング画面
	GS_TOWN,  // 町画面
	GS_SHOP,  // お店画面
	GS_SHOP_BUY,  // お店の買う画面
	GS_SHOP_SELL,  // お店の売る画面

	GS_DUNGEON,  // ダンジョン画面
	GS_BATTLE, // 戦闘画面
	GS_TREASURE, // 宝箱画面
	GS_STAIRS, // 階段みつけたよ画面

	GS_GAMEOVER, // ゲームオーバー画面

	GS_STATUS, // ステータス画面
	GS_USE_ITEM, // アイテム使用画面
	GS_USE_MAGIC, // 魔法使用画面

	GS_LOADING, // ローディング画面
};



// 関数のプロトタイプ宣言
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


// グローバル変数

// メッセージ・マスターデータ
char* gMsgMaster[] = {
	"戦闘エンカウント！",
	"宝箱を見つけた！",
	"階段を見つけた！",
	"ゲームオーバー！！"
};



// 宝箱は常に３択でアイテムを引っ張ってくる仕様
typedef struct {
	ITEM* item[3];
} TREASURE_ITEM;

// 宝箱アイテムマスター
TREASURE_ITEM gTreasureItemMaster[] = {
	{ NULL },  // ０階
	{ &gItemMaster[0], &gItemMaster[3], &gItemMaster[6]  },  // １階
	{ &gItemMaster[0], &gItemMaster[3], &gItemMaster[8] },  // ２階
	{ &gItemMaster[0], &gItemMaster[3], &gItemMaster[5] },  // ３階
	{ &gItemMaster[0], &gItemMaster[4], &gItemMaster[7] },  // ４階
};



int gGameStatus = GS_TOWN;  // 現在の画面を表す番号が入る変数　　※セーブ対象


PLAYER gPlayer;  // player.hで定義したPLAYER構造体型の変数を宣言　　※セーブ対象

#define MAX_PARTY_MEMBER  2
CHARACTER gParty[MAX_PARTY_MEMBER]; // パーティー全員のパラメータを持つ配列　　※セーブ対象

unsigned int gStartTime;


// メイン関数
void main(void) {

	// インベントリ配列をすべて０個にする
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		gPlayer.inventory[i] = 0;
	}

	gPlayer.gold = 500;

	gParty[0] = gCharaMaster[0];  // 勇者のLV1状態のパラメータをコピー
	gParty[1] = gCharaMaster[1];  // 戦士のLV1状態のパラメータをコピー

	srand((unsigned int)time(NULL));  // 乱数リストの初期化・生成

	load();

	while (gGameStatus != GS_APP_END) {  // ゲームループ

		switch (gGameStatus) {  // 画面切り替えのためのswitch文

		case GS_TITLE:  // タイトル画面の処理
			GsTitle();
			break;

		case GS_OPENING:  // オープニングの処理
			GsOpening();
			break;

		case GS_TOWN:  // 町の処理
			GsTown();
			break;

		case GS_SHOP:  // お店の処理
			GsShop();
			break;

		case GS_SHOP_BUY: // お店　アイテム買う
			GsShopBuy();
			break;

		case GS_SHOP_SELL: // お店　アイテム売る
			GsShopSell();
			break;

		case GS_DUNGEON: // ダンジョン
			GsDungeon();
			break;

		case GS_BATTLE: // 戦闘
			GsBattle();
			break;

		case GS_TREASURE: // 宝箱
			GsTreasure();
			break;

		case GS_STAIRS: // 階段みつけた画面
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

	// ↓　名前入力処理
	printf("名前を入力してください。 >>> ");
	scanf_s("%s", gPlayer.myName, 32);
	printf("おお！%sよ！よくぞ参った！\n", gPlayer.myName);
	// ↑  名前入力処理

	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("オープニング画面です\n");
	WaitForEnter();
	gGameStatus = GS_TOWN;
}


void GsTown(void) {
	int command;

	// 町に戻ったらHPとMP全回復
	Character_RecoverHpMax(&gParty[0]);
	Character_RecoverMpMax(&gParty[0]);
	Character_RecoverHpMax(&gParty[1]);
	Character_RecoverMpMax(&gParty[1]);

	system("cls");

	// 待ちの看板表示
	ShowTownBilboard();

	printf("コマンド一覧\n");
	printf("1. お店に行く\n");
	printf("2. ダンジョンに行く\n");
	printf("3. ステータス画面\n");
	printf("0. ゲーム終了\n");

	printf(">>> コマンド入力 >>>");
	scanf_s("%d", &command);

	if (command == 1) {  // お店に行くが選ばれた時
		gGameStatus = GS_SHOP;
	}
	else if (command == 2) {  // ダンジョンに行くが選ばれた時
	//	gGameStatus = GS_DUNGEON;
		gGameStatus = GS_LOADING;
		gStartTime = (unsigned int)time(NULL);
		gPlayer.floorNo = 1;  // フロア番号を１階にセット
	}
	else if (command == 3) {  // ステータス画面に行くが選ばれた時
		gGameStatus = GS_STATUS;
	}
	else if (command == 0) {  // ゲーム終了が選ばれた時
		gGameStatus = GS_APP_END;
	}
}


void GsShop(void) {
	int command;

	system("cls");

	// お店の看板表示（所持ゴールドも表示）
	ShowShopBilboard(gPlayer.gold);

	printf("1. 買う\n");
	printf("2. 売る\n");
	printf("3. やめる\n");

	printf("コマンド入力 >>>");
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
	// お店の看板表示（所持ゴールドも表示）
	ShowShopBilboard(gPlayer.gold);
	printf("商品一覧\n");

	// 商品リスト表示
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		// 番号　アイテム名　値段　を表示
		printf("%d %s   %d GOLD\n", i + 1, gItemMaster[i].name, gItemMaster[i].price);
	}

	printf("99. もどる\n");

	printf("どれを購入しますか？　番号で指定 >>>");
	scanf_s("%d", &command);

	if (command == 99) {  // もどるを選んだ時
		gGameStatus = GS_SHOP;
	}
	else if (command <= GetNumberOfItemMaster()) {  // 売ってるアイテムより大きな数字が入力されてたらはじく
		gPlayer.gold -= gItemMaster[command - 1].price;  // 購入されたアイテムの価格分、所持金を減らす
		gPlayer.inventory[command - 1]++;  // インベントリの所持アイテム個数を＋１する
	}
}


void GsShopSell(void) {
	int command;

	system("cls");
	// お店の看板表示（所持ゴールドも表示）
	ShowShopBilboard(gPlayer.gold);
	printf("どのアイテムを売りますか？\n");

	// 売るアイテムのリストを表示
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		// インベントリの個数が１個以上のアイテムだけ表示
		if (gPlayer.inventory[i] > 0) {
			// 番号　アイテム名　値段　を表示
			printf("%d %s   %d GOLD\n", i + 1, gItemMaster[i].name, gItemMaster[i].price / 2);
		}
	}

	printf("99. もどる\n");

	printf("どれを売りますか？　番号で指定 >>>");
	scanf_s("%d", &command);

	if (command == 99) {  // もどるを選んだ時
		gGameStatus = GS_SHOP;
	}
	else if (command <= GetNumberOfItemMaster()) {  // アイテムマスター最大値より大きな数字が入力されたらはじく
		if (gPlayer.inventory[command - 1] > 0) {  // インベントリ０個のアイテムが選択されたらはじく
			gPlayer.inventory[command - 1]--;  // インベントリの該当アイテムの個数を１個減らす
			gPlayer.gold += gItemMaster[command - 1].price / 2;  // 販売価格の1/2のゴールドを増やす
		}
	}
}


void GsDungeon(void) {
	int command;  // ANSI-Cの場合は関数の一番上に変数宣言を書く

	system("cls");
	ShowDungeonBillboard();

	printf("1. 奥に進む\n");
	printf("2. アイテムを使う\n");
	printf("3. 魔法を使う\n");
	printf("0. 町に戻る\n");

	printf("コマンド入力 >>>");

	scanf_s("%d", &command);

	if (command == 1) {
		// ３択で「戦闘・宝箱・階段」を決定
		int result = Get1of3(50, 45, 5);

		switch (result) {
		case 1:  // 戦闘
			gGameStatus = GS_BATTLE;
			break;
		case 2:  // 宝箱見つけた
			gGameStatus = GS_TREASURE;
			break;
		case 3:  // 階段
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

	// ターゲット決める処理
	for (;;) {  // 永久ループ
		CHARACTER* pTarget = &gParty[rand() % 2]; // ターゲットのキャラデータのアドレス取得

		if (pTarget->hp > 0) {  // ターゲットが生きてるかどうか
			pTarget->hp -= rand() % (pTarget->max_hp / 4); // ランダムでMAXHPの1/4のダメージ

			if (pTarget->hp < 0) {  // オーバーキルされたときにHPを０に戻す
				pTarget->hp = 0;
			}

			break;  // ループを即座に抜ける
		}
	}

	// ゲームオーバー判定
	if (gParty[0].hp <= 0) {  // HPが０以下になったら
		if (gParty[1].hp <= 0) {  // HPが０以下になったら
			gGameStatus = GS_GAMEOVER;  // ゲームオーバー画面に行く
			return;  // ゲームオーバーならこれ以降の処理を実行しない
		}
	}

	// EXP獲得処理
	int exp = rand() % 10 + 2;
	printf("EXPを%d獲得した！\n", exp);

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

	result = Get1of3(60, 30, 10) - 1;  // ３択からランダムで１つ数字を取ってくる

	getItem = gTreasureItemMaster[gPlayer.floorNo].item[result];  // 手に入れたアイテムのマスターデータのアドレスを取得

	printf("\n\n");
	printf("宝箱から「%s」を手に入れた！\n", getItem->name);

	gPlayer.inventory[getItem->itemNo]++;  // プレイヤーのインベントリの該当アイテムの個数を＋１

	WaitForEnter();
	gGameStatus = GS_DUNGEON;
}


void GsStairs(void) {
	system("cls");
	ShowMessage(2);
	WaitForEnter();
	gGameStatus = GS_DUNGEON;
	gPlayer.floorNo++;  // 階段を引いたら、現在の階層を＋１
}


void GsGameover(void)
{
	system("cls");
	ShowMessage(3);
	WaitForEnter();
	gGameStatus = GS_TITLE; // ゲームオーバー画面からはタイトル画面にしか行かない
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

	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("ステータス \n");
	printf("━━━━━━━━━━━━━━━━\n");

	printf("\n\n");
	printf("所持GOLD  %d\n", gPlayer.gold);
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

		//printf("ちから: %d\n", gParty[i].strength);

		printf("\n----------\n");
	}

	WaitForEnter();
	gGameStatus = GS_TOWN;
}

void GsUseItem(void)
{
	int command;

	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("アイテム使用画面 \n");
	printf("━━━━━━━━━━━━━━━━\n");

	printf("\n");

	// 使うアイテムのリストを表示
	for (int i = 0; i < GetNumberOfItemMaster(); i++) {
		// インベントリの個数が１個以上のアイテムだけ表示
		if (gPlayer.inventory[i] > 0) {
			// 番号　アイテム名　所持数　を表示
			printf("%d %s x %d\n", i + 1, gItemMaster[i].name, gPlayer.inventory[i]);
		}
	}

	printf("0. もどる\n");

	printf("どれを使いますか？　番号で指定 >>>");
	scanf_s("%d", &command);

	if (command == 0) {  // もどるを選んだ時
		gGameStatus = GS_DUNGEON;
	}
	else if (command <= GetNumberOfItemMaster()) {  // アイテムマスター最大値より大きな数字が入力されたらはじく
		if (gPlayer.inventory[command - 1] > 0) {  // インベントリ０個のアイテムが選択されたらはじく
			if (gItemMaster[command - 1].type == ITYPE_HPRECOVER) {
				gPlayer.inventory[command - 1]--;  // インベントリの該当アイテムの個数を１個減らす

				// アイテムの効果を発現させる
				printf("誰に使いますか？ [0.%s 1.%s] >>>", gParty[0].name, gParty[1].name);
				int charNo;
				scanf_s("%d", &charNo);

				gParty[charNo].hp += gItemMaster[command - 1].value;

				if (gParty[charNo].hp > gParty[charNo].max_hp) {
					gParty[charNo].hp = gParty[charNo].max_hp;
				}
			}
			else if (gItemMaster[command - 1].type == ITYPE_MPRECOVER) {
				gPlayer.inventory[command - 1]--;  // インベントリの該当アイテムの個数を１個減らす
			}
			else {

			}



		}
	}
}


void GsUseMagic(void)
{
	int command;
	MAGIC* pMagicUse;  // 使う魔法のマスターデータのアドレスを入れる変数
	CHARACTER* pCharOwner;  // 魔法使うキャラのアドレスを入れる変数
	CHARACTER* pCharTarget;  // 魔法を使う対象キャラのアドレスを入れる変数

	system("cls");
	printf("\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("魔法使用画面 \n");
	printf("━━━━━━━━━━━━━━━━\n");

	printf("\n");

	printf("どのキャラの魔法を使いますか？ [1.%s 2.%s 0.もどる] >>>", gParty[0].name, gParty[1].name);
	scanf_s("%d", &command);

	if (command == 0) {
		gGameStatus = GS_DUNGEON;
		return;
	}
	else if (command > 2) { // エラーチェック
		return;
	}

	pCharOwner = &gParty[command - 1];  // 魔法使用キャラのアドレスをセット

	// 使う魔法のリストを表示
	for (int i = 0; i < MAX_MAGIC_LIST; i++) {
		// すでに習得している魔法だけ表示
		if (pCharOwner->magicIDList[i] != 0) {  // 0は魔法スロットが空
			MAGIC* pMagic = GetMagic(pCharOwner->magicIDList[i]);
			// 番号　アイテム名　所持数　を表示
			printf("%d. ", i + 1);
			PrintAlignedStr(pMagic->name, 12);
			printf("MP %d\n", pMagic->mp);
		}
	}

	printf("0. もどる\n");

	printf("どれを使いますか？　番号で指定 >>>");
	scanf_s("%d", &command);

	pMagicUse = GetMagic(pCharOwner->magicIDList[command - 1]); // 使う魔法のアドレスをセット

	if (command == 0) {  // もどるを選んだ時
		return;
	}
	else if (command <= MAX_MAGIC_LIST) {
		if (pMagicUse->type == MTYPE_RECOVER) {  // 回復魔法は使える
			printf("誰に使いますか？ [1.%s 2.%s] >>>", gParty[0].name, gParty[1].name);
			scanf_s("%d", &command);

			pCharTarget = &gParty[command - 1];  // 使用対象キャラのアドレスをセット

			if (pCharOwner->mp >= pMagicUse->mp) {  // MP足りるかチェック
				Magic_Use(pMagicUse, pCharOwner, pCharTarget);
			}
			else {
				printf("MPが足りない！\n");
				WaitForEnter();
			}
		}
		else {
			printf("その魔法はここでは使えない！\n");
			WaitForEnter();
		}
	}
}


void GsLoading(void)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("Now Loading ... \n");
	printf("━━━━━━━━━━━━━━━━\n");

	if (time(NULL) > gStartTime + 1) {  // ローディング画面に来た瞬間の時間から、１秒経ったら
		gGameStatus = GS_DUNGEON;
		// オートセーブの場合は、ここでsave関数を呼び出すと良い
		save();
	}
}


void ShowMessage(int msgNo) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf(gMsgMaster[msgNo]);
	printf("\n");
	printf("━━━━━━━━━━━━━━━━\n");
}


void PrintAlignedStr(const char * str, int maxlen)
{
	printf(str);

	for (int i = 0; i < maxlen - (int)strlen(str); i++) {
		putc(' ', stdout);
	}
}



void ShowTownBilboard(void) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("ここは町です \n");
	printf("━━━━━━━━━━━━━━━━\n");
}


void ShowShopBilboard(int myGold) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("ここはお店です    所持金：%d GOLD\n", myGold);
	printf("━━━━━━━━━━━━━━━━\n");
}


void ShowDungeonBillboard(void) {
	printf("\n\n\n\n\n\n\n\n\n\n");  // 各画面決まった位置にメッセージを出すための改行
	printf("━━━━━━━━━━━━━━━━\n");
	printf("ダンジョン    %d 階\n", gPlayer.floorNo);
	printf("━━━━━━━━━━━━━━━━\n");

	printf("\n\n");

	printf("ステータス\n");

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



// ３択から１つを指定した確率で選ぶ関数
// 引数：
// int per1：選択肢１の抽選確率
// int per2：選択肢２の抽選確率
// int per3：選択肢３の抽選確率
// 戻り値：
// 抽選結果を整数で返す（１～３）
int Get1of3(int per1, int per2, int per3) {
	return Get1of4(per1, per2, per3, 0);
}


// ４択から１つを指定した確率で選ぶ関数
// 引数：
// int per1：選択肢１の抽選確率
// int per2：選択肢２の抽選確率
// int per3：選択肢３の抽選確率
// int per4：選択肢４の抽選確率
// 戻り値：
// 抽選結果を整数で返す（１～４）
int Get1of4(int per1, int per2, int per3, int per4) {
	int number = rand() % (per1 + per2 + per3 + per4);  // ４つの確率の合計値未満の乱数を取ってくる

	if (number < per1) {  // 確率per1の範囲なら
		return 1;
	}
	else if (number < per1 + per2) {  // 確率per2の範囲なら
		return 2;
	}
	else if (number < per1 + per2 + per3) {  // 確率per3の範囲なら
		return 3;
	}
	else {  //それ以外（per1でもper2でもない範囲）なら
		return 4;
	}
}

void save(void)
{
	FILE* fp;
	errno_t isOpen = fopen_s(&fp, "save.dat", "wb");  // バイナリモード

	if (isOpen == 0) {  // ファイルオープンが成功したら

		// データ書き込み処理
		// gGameStatusの書き込み
		fwrite(&gGameStatus, sizeof(gGameStatus), 1, fp);

		// gPlayerの書き込み
		fwrite(&gPlayer, sizeof(gPlayer), 1, fp);

		// gPartyの書き込み
		fwrite(&gParty[0], sizeof(CHARACTER), 2, fp);

		fclose(fp);

		printf("オートセーブ完了！");
	}
	else {
		printf("オートセーブ失敗！");
	}
}

void load(void)
{
	FILE* fp;
	errno_t isOpen = fopen_s(&fp, "save.dat", "rb");  // バイナリモード

	if (isOpen == 0) {  // ファイルオープンが成功したら

		// データ書き込み処理
		// gGameStatusの読み込み
		fread(&gGameStatus, sizeof(gGameStatus), 1, fp);

		// gPlayerの書き込み
		fread(&gPlayer, sizeof(gPlayer), 1, fp);

		// gPartyの書き込み
		fread(&gParty[0], sizeof(CHARACTER), 2, fp);

		fclose(fp);

		printf("オートロード完了！");
	}
	else {
		printf("オートロード失敗！");
	}
}
