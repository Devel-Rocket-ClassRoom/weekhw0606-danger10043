#include "Item.h"
#include "battle.h"
#include "basic.h"
#include "itemdata.h"
#include "Monster.h"
#include "World.h"

bool Item::isUsable() {
	return (ITEM_DATABASE[itemID].useFlag & USE_NORMAL) != 0;
}

bool Item::isBattleUsable() {
	return (ITEM_DATABASE[itemID].useFlag & USE_BATTLE) != 0;
}

void Item::useItem(World& world) {
	if (!isUsable()) {
		return;
	}
	ITEM_DATABASE[this->itemID].useFunc(*this, world);
}

void Item::battleUseItem(Player& player, Monster& enemy) {
	if (!isBattleUsable()) {
		return;
	}
	ITEM_DATABASE[this->itemID].battleFunc(player, enemy);
}