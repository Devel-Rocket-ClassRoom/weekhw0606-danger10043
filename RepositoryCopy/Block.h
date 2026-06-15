#pragma once
#include "Item.h"
#include <vector>

class World;

class Block : public Item {
public:
	int posR = 0, posC = 0;
	std::vector<int> ColorNoise = { 0, 0, 0 };

	static void printBlock(const Block& block, const World& world);

	void printBlock(const World& world);

	int getHardness();

	int getItem();
};