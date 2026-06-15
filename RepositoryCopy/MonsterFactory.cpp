#include "MonsterFactory.h"
#include "Monster.h"

// 이 아래부터 몬스터 추가시 수정할 것

std::unique_ptr<Monster> MonsterFactory::createMonster(int entityID) {

	switch (entityID) {
	case 5:
		return std::make_unique<WitherSkeleton>();
	case 50:
		return std::make_unique<Creeper>();
	case 51:
		return std::make_unique<Skeleton>();
	case 52:
		return std::make_unique<Spider>();
	case 54:
		return std::make_unique<Zombie>();
	case 56:
		return std::make_unique<Ghast>();
	case 58:
		return std::make_unique<Enderman>();
	case 61:
		return std::make_unique<Blaze>();
	case 62:
		return std::make_unique<MagmaCube>();
	case 63:
		return std::make_unique<EnderDragon>();
	case 90:
		return std::make_unique<Pig>();
	case 91:
		return std::make_unique<Sheep>();
	case 92:
		return std::make_unique<Cow>();
	default:
		return nullptr;
	}
}
