#include "Player.h"
#include "itemdata.h"
#include "colorprint.h"
#include "basic.h"
#include "Item.h"
#include "World.h"
#include <string>
#include <queue>

Player::Player() {
	Pos = { {0, 0}, {0, 0}, {0, 0} };
	ChanceMeetMonster = 80;
	HandItemID = 0;
	WeaponPower = 0;
	MiningPower = 0;
	doesNotMeetMonster = false;
	
	for (auto it = ITEM_DATABASE.begin(); it != ITEM_DATABASE.end(); it++) {
		int itemID = it->first;
		Inventory[itemID] = Item(itemID);
	}
}

int Player::GetDamage(int Damage, int ArmorPen) {
	int RealDefense = DefensePower * (100 - ArmorPen) / 100;
	int RealDamage = Damage * 100 / (100 + RealDefense);

	if (HasBlocked == 0) {
		RealDamage = 0;
		HasBlocked = 1;
	}

	RealDamage = RealDamage * (100 + 25 * BuffState[UNSTABLE]) / 100;
	Health -= RealDamage;

	if ((Equipment & (long long)PlayerEquipment::ACC3003) != 0) {
		Health += RealDamage * 3 / 10;
	}
	return RealDamage;
}

void Player::getItem(int itemId, int amount) {
	Inventory[itemId].count += amount;
}

void Player::loseItem(int itemId, int amount) {
	Inventory[itemId].count -= amount;
	if (Inventory[itemId].count < 0) {
		Inventory[itemId].count = 0;
	}
}

void Player::ShowInventory() {
	std::queue<std::string> inventoryPrintBuffer;
	std::string inventoryTempText = "";

	printf("\n");
	printUILine((INVENTORY_UI_WIDTH - 10) / 2, 1, 0, false);
	printf("[====== 인벤토리 ======]");
	printUILine(0, 1, (INVENTORY_UI_WIDTH - 10) / 2, true);
	printUILine(1, INVENTORY_UI_WIDTH - 2, 1, true);

	for (auto it = Inventory.begin(); it != Inventory.end(); it++) {
		const int& itemID = it->first;
		Item& targetItem = it->second;

		if (targetItem.count <= 0 || itemID == 0) {
			continue;
		}

		inventoryTempText = ColorText(ITEM_DATABASE[itemID].nameColor, ITEM_DATABASE[itemID].name);
		inventoryTempText += " : " + std::to_string(targetItem.count);
		while (getTextWidth(inventoryTempText) < (3 * INVENTORY_UI_WIDTH - 12) / 2) {
			inventoryTempText += " ";
		}
		inventoryPrintBuffer.push(inventoryTempText);
	}

	int totalRow = (static_cast<int>(inventoryPrintBuffer.size()) + 1) / 2;
	
	for (int row = 0; row < totalRow; row++) {
		if (inventoryPrintBuffer.empty()) {
			break;
		}
		printUILine(0, 2, 0, false);
		printf("%s", inventoryPrintBuffer.front().c_str());
		inventoryPrintBuffer.pop();
		if (inventoryPrintBuffer.empty()) {
			printf("\n");
			break;
		}
		printf("%s\n", inventoryPrintBuffer.front().c_str());
		inventoryPrintBuffer.pop();
	}
	printUILine(1, INVENTORY_UI_WIDTH - 2, 1, true);
	printUILine(INVENTORY_UI_WIDTH, 0, 0, true);
	printf("\n");
}

void Player::ShowStat() {
	TempTextLine = "체력 : [" + std::to_string(Health) + " / " + std::to_string(MaxHealth) + "]    /    " + "방어력 : " + std::to_string(DefensePower);
	TextBuffer.push(TempTextLine);
	TempTextLine = "채광력 : " + std::to_string(MiningPower) + "    /    " + "공격력 : " + std::to_string(AttackPower + WeaponPower);
	TextBuffer.push(TempTextLine);
	TempTextLine = "현재 손에 든 아이템 : " + ColorText(ITEM_DATABASE[HandItemID].nameColor, ITEM_DATABASE[HandItemID].name);
	TextBuffer.push(TempTextLine);
	TempTextLine.clear();
}

void Player::clearInventory(World& world) {
	Inventory.clear();
	Health = 200;
	HandItemID = 0;
	AttackPower = 10;
	WeaponPower = 0;
	MiningPower = 0;
	DefensePower = 0;
	Equipment = 0LL;

	BuffState = { 0, 0, 0, 0, 0 };
	world.world[OVERWORLD][Pos[OVERWORLD].first][Pos[OVERWORLD].second].itemID = 0;				
	world.world[NETHER][Pos[NETHER].first][Pos[NETHER].second].itemID = 0; 
	world.world[THE_END][Pos[THE_END].first][Pos[THE_END].second].itemID = 0;
	world.worldType = OVERWORLD;
	Pos[OVERWORLD] = { 7, WORLD_WIDTH / 2 };
	Pos[NETHER] = { 7, WORLD_WIDTH / 2 };
	Pos[THE_END] = { 7, WORLD_WIDTH / 2 };
	world.world[OVERWORLD][Pos[OVERWORLD].first][Pos[OVERWORLD].second].itemID = -1;
	world.world[NETHER][Pos[NETHER].first][Pos[NETHER].second].itemID = -1;
	world.world[THE_END][Pos[THE_END].first][Pos[THE_END].second].itemID = -1;

	TextBuffer.push("체력이 모두 소진되어 사망했습니다.");
	TextBuffer.push("모든 아이템을 잃고 스폰 위치로 복귀합니다.");
}
