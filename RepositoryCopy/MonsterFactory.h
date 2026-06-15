#pragma once
#include "Monster.h";
#include <iostream>

class MonsterFactory {
public:
	static std::unique_ptr<Monster> createMonster(int entityID);
};