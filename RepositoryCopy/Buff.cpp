#include "Buff.h"
#include "basic.h"
#include "Player.h"

std::vector<std::vector<int>> BuffColor = {
	{ 207, 33, 10 },
	{ 80, 181, 53 },
	{ 153, 153, 153 },
	{ 195, 87, 207 },
	{ 222, 98, 166 },
};

bool ApplyDebuff(Entity& Victim, Buff BuffType, int Amount) {
	if (Victim.BuffState[BuffType] == -1) {
		return false;
	}
	if (BuffType <= 2) {
		Victim.BuffState[BuffType] = Amount;
	}
	else {
		Victim.BuffState[BuffType] += Amount;
	}
	return true;
}

void EffectDebuff(Player& player) {
	bool HasDebuff = false;
	if (player.BuffState[FLAME] > 0) {
		int FlameDamage = player.MaxHealth * 5 / 100;
		TextBuffer.push("화염에 둘러싸여 " + std::to_string(player.GetDamage(FlameDamage, 100)) + "의 피해를 입었습니다.");
		player.BuffState[FLAME]--;
		if (player.BuffState[FLAME] == 0) {
			TextBuffer.push("플레이어에게 붙은 화염이 꺼졌습니다!");
		}
		HasDebuff = true;
	}

	if (player.BuffState[POISON] > 0) {
		int PoisonDamage = player.Health * 8 / 100;
		TextBuffer.push("중독 상태로 인해 " + std::to_string(player.GetDamage(PoisonDamage, 100)) + "의 피해를 입었습니다.");
		player.BuffState[POISON]--;
		if (player.BuffState[POISON] == 0) {
			TextBuffer.push("플레이어의 중독 상태가 해독되었습니다!");
		}
		HasDebuff = true;
	}

	if (player.BuffState[WITHER] > 0) {
		int WitherDamage = (std::max)(20, player.MaxHealth * 7 / 100);
		TextBuffer.push("시듦 상태로 인해 " + std::to_string(player.GetDamage(WitherDamage, 100)) + "의 피해를 입었습니다.");
		player.BuffState[WITHER]--;
		if (player.BuffState[WITHER] == 0) {
			TextBuffer.push("플레이어의 시듦 상태가 정화되었습니다!");
		}
		HasDebuff = true;
	}

	if (HasDebuff) {
		TextBuffer.push("플레이어의 현재 체력 : [ " + std::to_string(player.Health) + " / " + std::to_string(player.MaxHealth) + " ]");
	}
}
