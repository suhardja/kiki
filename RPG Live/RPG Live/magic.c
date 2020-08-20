#include "magic.h"
#include "character.h"



// 魔法マスター
MAGIC gMagicMaster[] = {

	{ "" },  // 0番は空にしておく
	{ "メラ", MTYPE_ATTACK, 10, FIRE, 3, 1 },
	{ "ホイミ", MTYPE_RECOVER, 30, NOELEMENT, 5, 2 },
	{ "ギガデイン", MTYPE_ATTACK, 300, THUNDER, 30, 34 }
};



void Magic_Use(MAGIC * pMg, CHARACTER * pChOwner, CHARACTER * pChTarget)
{
	switch (pMg->type) {
	case MTYPE_ATTACK:
		/////
		break;

	case MTYPE_RECOVER:
		Character_ConsumeMp(pChOwner, pMg->mp);
		Character_RecoverHp(pChTarget, pMg->value);
		break;
	}
}

int GetNumberOfMagicMaster(void)
{
	return sizeof(gMagicMaster) / sizeof(MAGIC);
}


MAGIC * GetMagic(int id)
{
	return gMagicMaster+id;
}
