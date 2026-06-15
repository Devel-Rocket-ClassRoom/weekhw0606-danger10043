#include "battle.h"
#include "Monster.h"
#include "basic.h"
#include "itemdata.h"
#include "item.h"
#include "colorprint.h"
#include "MonsterFactory.h"
#include "Buff.h"
#include "Player.h"
#include <vector>
#include <string>
#include <queue>
#include <windows.h>
#include <algorithm>
#include <iostream>

std::queue<std::string> battleTextBuffer;

void printBattleText() {
	printUILine(BATTLE_UI_WIDTH, 0, 0, true);
	printUILine(1, BATTLE_UI_WIDTH - 2, 1, true);
	while (!battleTextBuffer.empty()) {
		printUILine(0, 2, 0, false);
		printf("%s\n", battleTextBuffer.front().c_str());
		battleTextBuffer.pop();
	}
	printUILine(1, BATTLE_UI_WIDTH - 2, 1, true);
	printUILine(BATTLE_UI_WIDTH, 0, 0, true);
}
 
void playerAttack(Player& player, Monster* enemy) {
	if (enemy->entityID == 56) {
		battleTextBuffer.push("플레이어는 가스트에게 검을 휘둘렀다!");
		battleTextBuffer.push("그러나 공중의 가스트에게는 닿지 못했다..");
		return;
	}
	int damage = player.AttackPower + player.WeaponPower;
	int subDamage = (damage / 10) * (rand() % 5 - 2);
	damage += subDamage;
	battleTextBuffer.push("플레이어는 " + enemy->name + "에게 " + std::to_string(enemy->GetDamage(damage, enemy->DefensePower)) + " 의 피해를 입혔다!");
	
	if ((player.Equipment & (long long)PlayerEquipment::BOW3001) != 0) {
		battleTextBuffer.push("망령의 활이 적을 스스로 저격하여 " + std::to_string(enemy->GetDamage(player.AttackPower + player.WeaponPower, 0)) + " 의 피해를 입혔다!");
	}

	if ((player.Equipment & (long long)PlayerEquipment::ACC3002) != 0) {
		if (ApplyDebuff(*enemy, FLAME, 2)) {
			battleTextBuffer.push(enemy->name + "는(은) 불타기 시작했다!");
		}
	}
}

void printBattleInventory(Player& player) {
	std::queue<std::string> inventoryPrintBuffer;
	std::string inventoryTempText = "";

	for (auto it = player.Inventory.begin(); it != player.Inventory.end(); it++) {
		const int itemID = it->first;
		Item& targetItem = it->second;
		if (targetItem.count <= 0 || itemID == 0) {
			continue;
		}

		std::vector<int> textColor;
		if (targetItem.isBattleUsable()) {
			textColor = { 0, 255, 255 };
		}
		else {
			textColor = { 160, 160, 160 };
		}

		inventoryTempText = "      ";
		inventoryTempText += ColorText(textColor, ITEM_DATABASE[itemID].name + " : " + std::to_string(targetItem.count) + "개");
		while (getTextWidth(inventoryTempText) < (BATTLE_UI_WIDTH - 5)) {
			inventoryTempText += " ";
		}
		inventoryPrintBuffer.push(inventoryTempText);
	}
	
	int itemPrintCount = 0;
	inventoryTempText.clear();
	while (!inventoryPrintBuffer.empty()) {
		inventoryTempText += inventoryPrintBuffer.front();
		inventoryPrintBuffer.pop();
		itemPrintCount++;

		if (itemPrintCount % 3 == 0) {
			battleTextBuffer.push(inventoryTempText);
			inventoryTempText.clear();
		}
	}
	if (!inventoryTempText.empty()) {
		battleTextBuffer.push(inventoryTempText);
	}
	printBattleText();
}

void drawBattleScreen(Monster& enemy) {
	enemy.drawMonster();
	printf("\n");
	printBattleText();
}

std::vector<std::pair<int, int>> getBattleReward(Monster& enemy) {
	std::vector<std::pair<int, int>> result;

	std::map<int, int> rewardCountList;
	std::vector<std::pair<int, int>> rewardList = enemy.rewardList;

	for (std::pair<int, int> reward : rewardList) {
		int randomValue = rand() % 100 + 1;
		if (randomValue <= reward.first) {
			rewardCountList[reward.second]++;
		}
	}

	for (auto it = rewardCountList.begin(); it != rewardCountList.end(); it++) {
		result.push_back({ it->first, it->second });
	}

	return result;
}

bool battle(Player& player, int monsterID) {
	printf("<!> 몬스터와 조우했습니다 <!>\n\n");
	if ((player.Equipment & (long long)PlayerEquipment::SHIELD) != 0) {
		player.HasBlocked = 0;
	}

	Sleep(1500);
	std::unique_ptr<Monster> enemy = MonsterFactory::createMonster(monsterID);
	battleTextBuffer.push("야생의 " + enemy->name + "이(가) 나타났다!");
	while (true) {
		std::system("cls");
		battleTextBuffer.push("플레이어의 체력 : [ " + std::to_string(player.Health) + " / " + std::to_string(player.MaxHealth) + " ]  |  " + enemy->name + "의 체력 : [ " + std::to_string(enemy->Health) + " / " + std::to_string(enemy->MaxHealth) + " ]\n");

		if (*max_element(player.BuffState.begin(), player.BuffState.end()) > 0) {
			std::string TempBattleText = "플레이어 : ";
			if (player.BuffState[FLAME] > 0) {
				TempBattleText += ColorText({ 207, 33, 10 }, "불타오름");
				TempBattleText += "(" + std::to_string(player.BuffState[FLAME]) + "턴) ";
			}
			if (player.BuffState[POISON] > 0) {
				TempBattleText += ColorText({ 80, 181, 53 }, "중독");
				TempBattleText += "(" + std::to_string(player.BuffState[POISON]) + "턴) ";
			}
			if (player.BuffState[WITHER] > 0) {
				TempBattleText += ColorText({ 153, 153, 153 }, "시듦");
				TempBattleText += "(" + std::to_string(player.BuffState[WITHER]) + "턴) ";
			}
			if (player.BuffState[UNSTABLE] > 0) {
				TempBattleText += ColorText({ 195, 87, 207 }, "불안정함");
				TempBattleText += "(" + std::to_string(player.BuffState[UNSTABLE]) + "중첩) ";
			}
			if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
				TempBattleText += ColorText({ 222, 98, 166 }, "싹트는 수정");
				TempBattleText += "(" + std::to_string(player.BuffState[BLOOMING_CRYSTAL]) + "중첩) ";
			}
			battleTextBuffer.push(TempBattleText);
		}

		if (*max_element(enemy->BuffState.begin(), enemy->BuffState.end()) > 0) {
			std::string TempBattleText = enemy->name + " : ";
			if (enemy->BuffState[FLAME] > 0) {
				TempBattleText += ColorText({ 207, 33, 10 }, "불타오름");
				TempBattleText += "(" + std::to_string(enemy->BuffState[FLAME]) + "턴) ";
			}
			if (enemy->BuffState[POISON] > 0) {
				TempBattleText += ColorText({ 80, 181, 53 }, "중독");
				TempBattleText += "(" + std::to_string(enemy->BuffState[POISON]) + "턴) ";
			}
			if (enemy->BuffState[WITHER] > 0) {
				TempBattleText += ColorText({ 153, 153, 153 }, "시듦");
				TempBattleText += "(" + std::to_string(enemy->BuffState[WITHER]) + "턴) ";
			}
			if (enemy->BuffState[UNSTABLE] > 0) {
				TempBattleText += ColorText({ 195, 87, 207 }, "불안정함");
				TempBattleText += "(" + std::to_string(enemy->BuffState[UNSTABLE]) + "중첩) ";
			}
			if (enemy->BuffState[BLOOMING_CRYSTAL] > 0) {
				TempBattleText += ColorText({ 222, 98, 166 }, "싹트는 수정");
				TempBattleText += "(" + std::to_string(enemy->BuffState[BLOOMING_CRYSTAL]) + "중첩) ";
			}
			battleTextBuffer.push(TempBattleText);
		}

		battleTextBuffer.push("행동을 입력하세요 < [1] 공격  /  [2] 아이템 사용  /  [3] 도망 >");
		drawBattleScreen(*enemy);
		printf("행동 입력 : ");
		int playerAction = 0; std::cin >> playerAction;
		// 플레이어 공격
		if (std::cin.fail() || ((playerAction <= 0 || playerAction >= 4) && (playerAction < 10043 || playerAction > 10045))) {
			std::system("cls");
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			battleTextBuffer.push("잘못된 입력입니다.");
			drawBattleScreen(*enemy);
			continue;
		}
		if (playerAction == 1) {
			std::system("cls");
			playerAttack(player, enemy.get());
			drawBattleScreen(*enemy);
			Sleep(2000);
		}
		// 플레이어 아이템 사용
		else if (playerAction == 2) {
			std::system("cls");
			enemy->drawMonster();
			printf("\n");
			printBattleInventory(player);
			printf("사용할 아이템의 이름을 입력하세요. 취소하려면 X를 입력하세요.\n");
			printf("입력 : ");
			/*std::string Input; std::cin.ignore(); std::getline(std::cin, Input);*/
			std::string Input; Input = InputString();

			if (Input == "X" || Input == "x") {
				continue;
			}

			Item& targetItem = player.Inventory[ItemName2ItemID[Input]];
			if (ItemName2ItemID[Input] == 0 || targetItem.count <= 0 || (!targetItem.isBattleUsable())) {
				battleTextBuffer.push("보유하지 않은 아이템이거나 사용 가능한 아이템이 아닙니다.");
				std::system("cls");
				drawBattleScreen(*enemy);
				Sleep(2000);
				continue;
			}

			if (Input == "엔더 진주") {
				if (enemy->entityID == 63) {
					battleTextBuffer.push("엔더 드래곤으로부터 도망칠 수 없습니다!");
				}
				else {
					std::system("cls");
					player.loseItem(ItemName2ItemID[Input], 1);
					battleTextBuffer.push("엔더 진주를 사용하여 도망에 성공했습니다!");
					drawBattleScreen(*enemy);
					Sleep(2500);
					return true;
				}
			}

			std::system("cls");
			player.loseItem(ItemName2ItemID[Input], 1);
			targetItem.battleUseItem(player, *enemy);
			drawBattleScreen(*enemy);
			Sleep(2000);
		}
		// 도망
		else if (playerAction == 3) {
			if (enemy->entityID == 63) {
				battleTextBuffer.push("엔더 드래곤으로부터 도망칠 수 없습니다!");
			}
			else {
				int randomValue = rand() % 1000;
				if (randomValue < 350) {
					std::system("cls");
					battleTextBuffer.push("도망에 성공했습니다!");
					drawBattleScreen(*enemy);
					Sleep(2500);
					return true;
				}
				else {
					std::system("cls");
					battleTextBuffer.push("도망치는데 실패했습니다..");
					drawBattleScreen(*enemy);
					Sleep(2500);
				}
			}
		}
		else if (playerAction == 10043) {
			battleTextBuffer.push("플레이어는 " + enemy->name + "에게 죽창을 꽂아 " + std::to_string(enemy->GetDamage(99999, 100)) + " 의 피해를 입혔다!");
		}
		else if (playerAction == 10044) {
			printf("플레이어에게 디버프를 겁니다. 디버프 코드를 입력하세요.\n");
			int Debuff; std::cin >> Debuff;
			ApplyDebuff(player, (Buff)Debuff, 1);
		}
		else if (playerAction == 10045) {
			printf("적에게 디버프를 겁니다. 디버프 코드를 입력하세요.\n");
			int Debuff; std::cin >> Debuff;
			ApplyDebuff(*enemy, (Buff)Debuff, 1);
		}

		// 플레이어 디버프 적용
		for (int i = 0; i <= 4; i++) {
			if (player.BuffState[i] > 0) {
				std::system("cls");
				ApplyBattleDebuffEffect(player, (Buff)i);
				if (i <= 2) {
					player.BuffState[i]--;
					if (player.BuffState[i] == 0) {
						if (i == 0) {
							battleTextBuffer.push("플레이어에게 붙은 불이 사그라들었습니다!");
						}
						else if (i == 1) {
							battleTextBuffer.push("플레이어의 중독 상태가 해독되었습니다!");
						}
						else if (i == 2) {
							battleTextBuffer.push("플레이어의 시듦 상태가 치유되었습니다!");
						}
					}
				}
				drawBattleScreen(*enemy);
				Sleep(2500);
			}
		}

		// 적 디버프 적용
		for (int i = 0; i <= 4; i++) {
			if (enemy->BuffState[i] > 0) {
				std::system("cls");
				ApplyBattleDebuffEffect(*enemy, (Buff)i);
				if (i <= 2) {
					enemy->BuffState[i]--;
					if (enemy->BuffState[i] == 0) {
						if (i == 0) {
							battleTextBuffer.push(enemy->name + "에게 붙은 불이 사그라들었습니다!");
						}
						else if (i == 1) {
							battleTextBuffer.push(enemy->name + "의 중독 상태가 해독되었습니다!");
						}
						else if (i == 2) {
							battleTextBuffer.push(enemy->name + "플레이어의 시듦 상태가 치유되었습니다!");
						}
					}
				}
				drawBattleScreen(*enemy);
				Sleep(2500);
			}
		}
		if (enemy->entityID == 62 && enemy->Health <= 0) {
			MagmaCube* MagmaEnemy = dynamic_cast<MagmaCube*>(&*enemy);
			if (MagmaEnemy->Size > 0) {
				std::system("cls");
				enemy->Health = enemy->MaxHealth;
				MagmaEnemy->Size--;
				battleTextBuffer.push("마그마 크림이 분열하여 완전히 회복했다!");
				battleTextBuffer.push("남은 회복 횟수 : " + std::to_string(MagmaEnemy->Size));
				drawBattleScreen(*enemy);
				Sleep(2000);
			}
		}
		if (enemy->Health <= 0) {
			std::system("cls");
			battleTextBuffer.push(enemy->name + "와(과)의 전투에서 승리했습니다!");
			drawBattleScreen(*enemy);
			Sleep(2500);
			std::system("cls");
			TempTextLine = "";
			std::vector<std::pair<int, int>> rewardList = getBattleReward(*enemy);
			for (std::pair<int, int> reward : rewardList) {
				TempTextLine += ITEM_DATABASE[reward.first].name + " " + std::to_string(reward.second) + "개, ";
				player.getItem(reward.first, reward.second);
			}
			if (!TempTextLine.empty()) {
				TempTextLine.pop_back();
				TempTextLine.pop_back();
				TempTextLine += "를 획득했습니다!";
				battleTextBuffer.push(TempTextLine);
				drawBattleScreen(*enemy);
				Sleep(2500);
			}
			else {
				drawBattleScreen(*enemy);
			}
			return true;
		}
		enemy->attackSkill(player);
		std::system("cls");
		drawBattleScreen(*enemy);
		Sleep(2000);
		if (player.Health <= 0) {
			std::system("cls");
			battleTextBuffer.push("플레이어가 " + enemy->name + "에게 사망했습니다.");battleTextBuffer.push("인벤토리의 모든 아이템을 잃었습니다.");
			drawBattleScreen(*enemy);
			Sleep(2000);
			return false;
		}
		if (enemy->Health <= 0) {
			std::system("cls");
			battleTextBuffer.push(enemy->name + "와(과)의 전투에서 승리했습니다!");
			drawBattleScreen(*enemy);
			Sleep(2500);
			std::system("cls");
			TempTextLine = "";
			std::vector<std::pair<int, int>> rewardList = getBattleReward(*enemy);
			for (std::pair<int, int> reward : rewardList) {
				TempTextLine += ITEM_DATABASE[reward.first].name + " " + std::to_string(reward.second) + "개, ";
				player.getItem(reward.first, reward.second);
			}
			if (!TempTextLine.empty()) {
				TempTextLine.pop_back();
				TempTextLine.pop_back();
				TempTextLine += "를 획득했습니다!";
				battleTextBuffer.push(TempTextLine);
				drawBattleScreen(*enemy);
				Sleep(2500);
			}
			else {
				drawBattleScreen(*enemy);
			}
			return true;
		}
	}
}

void ApplyBattleDebuffEffect(Entity& Victim, Buff BuffType) {
	if (BuffType == FLAME) {
		int FlameDamage = Victim.MaxHealth * 5 / 100;
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는(은) 화염에 휩싸였다!"
		);
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는(은) " + std::to_string(Victim.GetDamage(FlameDamage, 100)) + "의 피해를 입었다."
		);
	}

	else if (BuffType == POISON) {
		int PoisonDamage = Victim.Health * 8 / 100;
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는 중독 상태에 시달리고 있다!"
		);
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는(은) " + std::to_string(Victim.GetDamage(PoisonDamage, 100)) + "의 피해를 입었다."
		);
	}

	else if (BuffType == WITHER) {
		int WitherDamage = (std::max)(20, Victim.MaxHealth * 7 / 100);
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는 시들고 있다.."
		);
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는(은) " + std::to_string(Victim.GetDamage(WitherDamage, 100)) + "의 피해를 입었다."
		);
	}

	else if (BuffType == UNSTABLE) {
		if (Victim.BuffState[UNSTABLE] == 1) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "의 주위는 불안정하게 일그러져 있다."
			);
		}
		if (Victim.BuffState[UNSTABLE] == 2) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "의 형체가 불규칙하게 흔들린다."
			);
		}
		if (Victim.BuffState[UNSTABLE] == 3) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "의 주위 공간에 균열이 번져나간다."
			);
		}
		if (Victim.BuffState[UNSTABLE] == 4) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "를(을) 둘러싼 공간이 붕괴 직전까지 흔들리고 있다."
			);
		}
	}

	else if (BuffType == BLOOMING_CRYSTAL) {
		int CrystalDamage = 5 * Victim.BuffState[BLOOMING_CRYSTAL];
		if (Victim.BuffState[BLOOMING_CRYSTAL] == 1) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "싹트는 수정의 피해를 입는다!"
			);
		}

		if (Victim.BuffState[BLOOMING_CRYSTAL] == 2) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "싹트는 수정의 피해를 입는다!"
			);
		}

		if (Victim.BuffState[BLOOMING_CRYSTAL] == 3) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "싹트는 수정의 피해를 입는다!"
			);
		}

		if (Victim.BuffState[BLOOMING_CRYSTAL] == 4) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "싹트는 수정의 피해를 입는다!"
			);
		}

		if (Victim.BuffState[BLOOMING_CRYSTAL] == 5) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "싹트는 수정의 피해를 입는다!"
			);
		}

		if (Victim.BuffState[BLOOMING_CRYSTAL] == 6) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "싹트는 수정의 피해를 입는다!"
			);
		}

		if (Victim.BuffState[BLOOMING_CRYSTAL] == 7) {
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "몸 안의 엔드 수정이 폭발했다!"
			);
			battleTextBuffer.push(
				((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는(은) " + std::to_string(Victim.GetDamage(Victim.MaxHealth * 3, 100)) + "의 피해를 입었다."
			);
			return;
		}
		battleTextBuffer.push(
			((Victim.GetType() == EntityType::PLAYER) ? "플레이어" : static_cast<Monster*>(&Victim)->name) + "는(은) " + std::to_string(Victim.GetDamage(CrystalDamage, 0)) + "의 피해를 입었다."
		);
	}
}