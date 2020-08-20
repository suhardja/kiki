#include "item.h"


// アイテムマスターの配列
ITEM gItemMaster[] = {
	{ ITYPE_HPRECOVER, 0, "薬草", 10, 8 },			// 0
	{ ITYPE_MPRECOVER, 1, "魔法の小瓶", 10, 50 },
	{ ITYPE_WEAPON, 2, "はがねの剣", 80, 300 },
	{ ITYPE_ETC, 3, "せいすい", 10, 30 },
	{ ITYPE_ETC, 4, "せかいじゅの葉", 10, 1000 },
	{ ITYPE_ETC, 5, "ちからの種", 10, 500 },			// 5
	{ ITYPE_ETC, 6, "どくけしそう", 10, 15 },
	{ ITYPE_WEAPON, 7, "ドラゴンバスター", 10, 30000 },
	{ ITYPE_WEAPON, 8, "どうの大剣", 10, 30 },			// 8
};


int GetNumberOfItemMaster(void)
{
	return sizeof(gItemMaster) / sizeof(ITEM);
}
