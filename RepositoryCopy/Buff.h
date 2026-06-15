#pragma once
#include <vector>

class Player;


enum Buff {
	FLAME = 0,
	POISON = 1,
	WITHER = 2,
	UNSTABLE = 3,
	BLOOMING_CRYSTAL = 4,
};

extern std::vector<std::vector<int>> BuffColor;

enum class EntityType {
	PLAYER = 0,
	MONSTER = 1,
};

class Entity {
public:
	std::vector<int> BuffState = { 0, 0, 0, 0, 0 };
	int Health = 200;
	int MaxHealth = 200;
	int AttackPower = 10;
	int DefensePower = 0;
	int ArmorPower = 0;

	int GetHeal(int Heal) {
		int RealHeal = (std::min)(MaxHealth - Health, Heal);
		Health += RealHeal;
		return RealHeal;
	}

	virtual int GetDamage(int Damage, int ArmorPen) {
		int RealDefense = DefensePower * (100 - ArmorPen) / 100;
		int RealDamage = Damage * 100 / (100 + RealDefense);
		RealDamage = RealDamage * (100 + 25 * BuffState[UNSTABLE]) / 100;
		Health -= RealDamage;
		return RealDamage;
	}

	virtual EntityType GetType() = 0;
};

bool ApplyDebuff(Entity& Victim, Buff BuffType, int Amount);

void EffectDebuff(Player& player);