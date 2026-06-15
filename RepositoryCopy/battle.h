#pragma once
#include "Buff.h"
#include <queue>
#include <string>

class Player;
class Monster;

extern std::queue<std::string> battleTextBuffer;

void printBattleText();

void playerAttack(Player& player, Monster* enemy);

void printBattleInventory(Player& player);

void drawBattleScreen(Monster& enemy);

std::vector<std::pair<int, int>> getBattleReward(Monster& enemy);

bool battle(Player& player, int monsterID);

void ApplyBattleDebuffEffect(Entity& Victim, Buff BuffType);