#pragma once
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include "Buff.h"

class Player;

class Monster : public Entity {
public:
	int entityID = 0;
	std::string name = "";
	std::vector<std::pair<int, int>> rewardList;

	EntityType GetType() override {
		return EntityType::MONSTER;
	}

	void setStat(int hp, int ad, int armor);

	virtual ~Monster() = default;

	virtual void drawMonster() = 0;

	virtual void attackSkill(Player& player) = 0;

};

// 이 아래부터 몬스터 추가시 수정할 것

// 5, 위더 스켈레톤
class WitherSkeleton : public Monster {
public:
	WitherSkeleton() : Monster() {
		entityID = 5;
		setStat(200, 25, 20);
		name = "위더 스켈레톤";
		rewardList = { {100, 263}, {100, 263}, {50, 263}, {50, 263}, {100, 352}, {100, 352}, {50, 352}, {50, 352}, };
		BuffState[FLAME] = -1;
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 50, 크리퍼
class Creeper : public Monster {
public:
	int fuse = 3;
	Creeper() : Monster() {
		entityID = 50;
		setStat(100, 150, 0);
		name = "크리퍼";
		rewardList = { {100, 289}, {100, 289}, {50, 289}, {50, 289} };
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 51, 스켈레톤
class Skeleton : public Monster {
public:
	Skeleton() : Monster() {
		entityID = 51;
		setStat(110, 20, 0);
		name = "스켈레톤";
		rewardList = { {100, 262}, {100, 262}, {50, 262}, {100, 352}, {100, 352}, {100, 352}, {100, 352}, {50, 352}, {50, 352}, {50, 352}, };
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 52, 거미
class Spider : public Monster {
public:
	int attackDecreaseAmount = 0;
	Spider() : Monster() {
		entityID = 52;
		setStat(90, 18, 0);
		name = "거미";
		rewardList = { {100, 287}, {100, 287}, {50, 287}, {50, 287}, };
	};

	int GetDamage(int Damage, int ArmorPen) {
		int RealDefense = DefensePower * (100 - ArmorPen) / 100;
		int RealDamage = Damage * 100 / (100 + RealDefense);
		Health -= (std::max)(1, Damage - attackDecreaseAmount);
		return (std::max)(1, Damage - attackDecreaseAmount);
	}

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 54, 좀비
class Zombie : public Monster {
public:
	Zombie() : Monster() {
		entityID = 54;
		setStat(130, 15, 0);
		name = "좀비";
		rewardList = { {100, 367}, {70, 367}, {30, 367}, {5, 367}, };
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 56, 가스트
class Ghast : public Monster {
public:
	Ghast() : Monster() {
		entityID = 56;
		setStat(50, 60, 0);
		name = "가스트";
		rewardList = { {100, 370}, {50, 370}, {50, 370},};
		BuffState[FLAME] = -1;
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 58, 엔더맨
class Enderman : public Monster {
public:
	bool isUpset = false;

	Enderman() : Monster() {
		entityID = 58;
		setStat(200, 30, 0);
		name = "엔더맨";
		rewardList = { {100, 368}, {100, 368}, {100, 368}, {35, 368}, };
	};

	int GetDamage (int Damage, int ArmorPen) override;
	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 61, 블레이즈
class Blaze : public Monster {
public:
	int Charge = 0;
	Blaze() : Monster() {
		entityID = 61;
		setStat(230, 45, 20);
		name = "블레이즈";
		rewardList = { {100, 369}, {70, 369}, {30, 369}, };
		BuffState[FLAME] = -1;
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 62, 마그마 큐브
class MagmaCube : public Monster {
public:
	int Size = 2;
	MagmaCube() : Monster() {
		entityID = 62;
		setStat(70, 50, 0);
		name = "마그마 크림";
		rewardList = { {100, 378}, {50, 378}, {50, 378} };
		BuffState[FLAME] = -1;
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 63, 엔더 드래곤
class EnderDragon : public Monster {
public:
	int BreathDuration = -2;
	EnderDragon() : Monster() {
		entityID = 63;
		setStat(2500, 70, 100);
		name = "엔더 드래곤";
		rewardList = {};
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 90, 돼지
class Pig : public Monster {
public:
	Pig() : Monster() {
		entityID = 90;
		setStat(60, 0, 0);
		name = "돼지";
		rewardList = { {100, 319}, {70, 319}, {30, 319}, };
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 91, 양
class Sheep : public Monster {
public:
	Sheep() : Monster() {
		entityID = 91;
		setStat(60, 0, 0);
		name = "양";
		rewardList = { {100, 423}, {70, 423}, {30, 423}, };
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};

// 92, 소
class Cow : public Monster {
public:
	Cow() : Monster() {
		entityID = 92;
		setStat(60, 0, 0);
		name = "소";
		rewardList = { {100, 363}, {70, 363}, {30, 363}, };
	};

	void drawMonster() override;
	void attackSkill(Player& player) override;
};




