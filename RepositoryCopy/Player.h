#pragma once
#include "Item.h"
#include "Buff.h"
#include <map>
#include <vector>

class World;
class Item;

enum class PlayerEquipment : long long {
	SHIELD = 1LL << 0,
	BOW3001 = 1LL << 1,
	ACC3002 = 1LL << 2,
	ACC3003 = 1LL << 3,
};

class Player : public Entity {
public:
	std::vector<std::pair<int, int>> Pos;

	EntityType GetType() override {
		return EntityType::PLAYER;
	}

	std::map<int, Item> Inventory;

	int ChanceMeetMonster = 80;
	int HandItemID = 0;
	int WeaponPower = 0;
	int MiningPower = 0;
	bool doesNotMeetMonster = false;

	long long Equipment = 0;
	int HasBlocked = -1;

	Player();

	int GetDamage(int Damage, int ArmorPen) override;

	void getItem(int itemId, int amount);

	void loseItem(int itemId, int amount);

	void ShowInventory();

	void ShowStat();

	void clearInventory(World& world);
};