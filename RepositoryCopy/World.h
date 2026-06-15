#pragma once
#include "Player.h"
#include "Block.h"
#include <vector>

extern const std::vector<int> DAWN;
extern const std::vector<int> NOON;
extern const std::vector<int> DUSK;
extern const std::vector<int> MIDNIGHT;

enum WorldType {
	OVERWORLD = 0,
	NETHER = 1,
	THE_END = 2,
};

class World {
public:
	std::vector<std::vector<std::vector<Block>>> world;
	int currentTime = 50;
	Player player;
	WorldType worldType = OVERWORLD;

	void SwitchItem();

	void PlaceBlock();

	void printCraft();

	void craft();

	void giveItem();

	void printWorld();

	bool playerMine(int worldPosR, int worldPosC);

	void playerMove(char direction);

	// (입력한 수 / 100)% 확률로 전투를 시작하는 함수.
	bool tryBattle(int Chance);
	bool tryBattle();

	void useItemInWorld();

	void getItemInfo();

	void updateLiquid(); 

	bool IsPlayerNearbyBlock(int BlockID);

	void TP();
};