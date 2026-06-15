#pragma once
#include "itemdata.h"

class World;
class Player;
class Monster;

class Item {
public:
	int itemID = 0;
	int count = 0;

	Item() : itemID(0), count(0) {};
	Item(int id) : itemID(id), count(0) {};

	bool isUsable();

	bool isBattleUsable();

	void useItem(World& world);

	void battleUseItem(Player& player, Monster& enemy);

	Item& operator++() {
		count++;
		return *this;
	}

	Item& operator--() {
		count--;
		return *this;
	}
};


