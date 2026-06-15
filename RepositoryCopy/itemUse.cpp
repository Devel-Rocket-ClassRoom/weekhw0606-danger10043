#include "itemUse.h"
#include "Player.h"
#include "Monster.h"
#include "basic.h"
#include "battle.h"
#include "World.h"
#include "random.h"
#include "colorprint.h"
#include <iostream>
#include <queue>

void useItem259(Item& item, World& targetWorld) {
	std::vector<std::vector<Block>>& world = targetWorld.world[targetWorld.worldType];
	Player& player = targetWorld.player;

	player.getItem(259, 1);
	TextBuffer.push("라이터를 사용할 방향을 입력하세요.");
	TextBuffer.push("( 플레이어 위치를 기준으로, 상:w, 좌:a, 하:s, 우:d, 취소:x )");
	targetWorld.printWorld();
	printf("입력 : ");

	char Direction; std::cin >> Direction;

	int TargetPosRow = player.Pos[targetWorld.worldType].first;
	int TargetPosCol = player.Pos[targetWorld.worldType].second;
	if (Direction == 'w' || Direction == 'W') {
		TargetPosRow--;
		if (TargetPosRow < 1 || world[TargetPosRow][TargetPosCol].itemID != 49) {
			TextBuffer.push("해당 위치에는 라이터를 사용할 수 없습니다.");
			return;
		}
		TargetPosRow--;
	}
	else if (Direction == 'a' || Direction == 'A') {
		TargetPosCol--;
		if (TargetPosCol < 1 || world[TargetPosRow][TargetPosCol].itemID != 49) {
			TextBuffer.push("해당 위치에는 라이터를 사용할 수 없습니다.");
			return;
		}
		TargetPosCol--;
	}
	else if (Direction == 's' || Direction == 'S') {
		TargetPosRow++;
		if (TargetPosRow > WORLD_HEIGHT - 2 || world[TargetPosRow][TargetPosCol].itemID != 49) {
			TextBuffer.push("해당 위치에는 라이터를 사용할 수 없습니다.");
			return;
		}
		TargetPosRow++;
	}
	else if (Direction == 'd' || Direction == 'D') {
		TargetPosCol++;
		if (TargetPosCol > WORLD_WIDTH - 2 || world[TargetPosRow][TargetPosCol].itemID != 49) {
			TextBuffer.push("해당 위치에는 라이터를 사용할 수 없습니다.");
			return;
		}
		TargetPosCol++;
	}

	if (world[TargetPosRow][TargetPosCol].itemID != 0) {
		TextBuffer.push("해당 위치에는 라이터를 사용할 수 없습니다.");
		return;
	}

	int EmptyCount = 0;
	std::vector<std::vector<bool>> Visited(WORLD_HEIGHT, std::vector<bool>(WORLD_WIDTH, false));
	std::queue<std::pair<int, int>> Queue;
	Queue.push({ TargetPosRow, TargetPosCol });

	while (!Queue.empty()) {
		int Row = Queue.front().first;
		int Col = Queue.front().second;
		Queue.pop();

		if (Visited[Row][Col]) {
			continue;
		}
		Visited[Row][Col] = true;
		EmptyCount++;
		if (world[Row][Col].itemID != 0 && world[Row][Col].itemID != 49) {
			TextBuffer.push("포탈 생성 위치의 내부가 비어있지 않거나,");
			TextBuffer.push("해당 공간이 흑요석으로 닫혀 있지 않습니다.");
			return;
		}

		for (int dr = -1; dr <= 1; dr++) {
			for (int dc = -1; dc <= 1; dc++) {
				if (dr * dc != 0 || dr + dc == 0) {
					continue;
				}
				if (Row + dr < 0 || Row + dr >= WORLD_HEIGHT) {
					continue;
				}
				if (Col + dc < 0 || Col + dc >= WORLD_WIDTH) {
					continue;
				}
				if (Visited[Row + dr][Col + dc]) {
					continue;
				}
				if (world[Row + dr][Col + dc].itemID == 49) {
					continue;
				}
				Queue.push({ Row + dr, Col + dc });
			}
		}
	}
	if (EmptyCount < 8) {
		TextBuffer.push("흑요석으로 닫힌 공간이 너무 작습니다.");
		return;
	}

	for (int r = 0; r < WORLD_HEIGHT; r++) {
		for (int c = 0; c < WORLD_WIDTH; c++) {
			if (Visited[r][c])
				world[r][c].itemID = 90;
		}
	}
	TextBuffer.push("네더 포탈을 생성했습니다!");
}

void battleUseItem259(Player& player, Monster& enemy) {
}

void battleUseItem261(Player& player, Monster& enemy) {
	player.getItem(261, 1);
	if (player.Inventory[262].count <= 0) {
		battleTextBuffer.push("화살이 없어 활로 " + enemy.name + "를 후려쳤다.");
		int damage = (player.AttackPower + player.WeaponPower) * 8 / 10;
		battleTextBuffer.push("플레이어는 " + enemy.name + "에게 " + std::to_string(enemy.GetDamage(damage, 0)) + " 의 피해를 입혔다!");
		return;
	}
	player.loseItem(262, 1);

	if (enemy.entityID == 58) {
		battleTextBuffer.push("플레이어는 활을 쏘았으나, 엔더맨은 회피했다.");
		enemy.GetDamage(0, 0);
		return;
	}

	if (enemy.MaxHealth == enemy.Health) {
		battleTextBuffer.push("플레이어는 활을 사용하여 강력한 원거리 저격을 쏘았다!");
		int damage = (player.AttackPower + player.WeaponPower) * 5 / 2;
		battleTextBuffer.push("플레이어는 " + enemy.name + "에게 " + std::to_string(enemy.GetDamage(damage, 0)) + " 의 피해를 입혔다!");
	}
	else {
		battleTextBuffer.push("플레이어는 활을 사용하여 화살을 쏘았다.");
		int damage = (player.AttackPower + player.WeaponPower) * 3 / 2;
		battleTextBuffer.push("플레이어는 " + enemy.name + "에게 " + std::to_string(enemy.GetDamage(damage, 0)) + " 의 피해를 입혔다!");
	}
}

void useItem319(Item& item, World& world) {
	TextBuffer.push("돼지고기를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(40)) + " 회복했습니다.");
}
void battleUseItem319(Player& player, Monster& enemy) {
	battleTextBuffer.push("돼지고기를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(40)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem320(Item& item, World& world) {
	TextBuffer.push("익힌 돼지고기를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(100)) + " 회복했습니다.");
}
void battleUseItem320(Player& player, Monster& enemy) {
	battleTextBuffer.push("익힌 돼지고기를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(100)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem346(Item& item, World& world) {
	Player& player = world.player;
	player.getItem(346, 1);

	// 플레이어가 물과 인접해 있는지 검사
	bool IsPlayerNearbyWater = world.IsPlayerNearbyBlock(8);

	if (!IsPlayerNearbyWater) {
		TextBuffer.push("플레이어가 물 블럭에 인접해있지 않습니다.");
		return;
	}

	int RandomValue = RandomIntRange(1, 100);
	if (RandomValue <= 85) {
		int FishCount = RandomIntRange(1, 3);
		TextBuffer.push("[대구] " + std::to_string(FishCount) + " 개를 획득했습니다!");
		player.getItem(349, FishCount);
		TextBuffer.push("현재 대구 보유 개수 : " + std::to_string(player.Inventory[349].count) + "개");
	}
	else {
		if (!world.tryBattle(10000)) {
			player.clearInventory(world);
		}
	}
}

void useItem349(Item& item, World& world) {
	TextBuffer.push("대구를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(30)) + " 회복했습니다.");
}

void battleUseItem349(Player& player, Monster& enemy) {
	battleTextBuffer.push("대구를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(30)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem350(Item& item, World& world) {
	TextBuffer.push("익힌 대구를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(80)) + " 회복했습니다.");
}

void battleUseItem350(Player& player, Monster& enemy) {
	battleTextBuffer.push("익힌 대구를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(80)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem363(Item& item, World& world) {
	TextBuffer.push("소고기를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(40)) + " 회복했습니다.");
}
void battleUseItem363(Player& player, Monster& enemy) {
	battleTextBuffer.push("소고기를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(40)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem364(Item& item, World& world) {
	TextBuffer.push("스테이크를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(100)) + " 회복했습니다.");
}
void battleUseItem364(Player& player, Monster& enemy) {
	battleTextBuffer.push("스테이크를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(100)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem367(Item& item, World& world) {
	TextBuffer.push("썩은 고기를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(20)) + " 회복했습니다.");
}
void battleUseItem367(Player& player, Monster& enemy) {
	battleTextBuffer.push("썩은 고기를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(20)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem368(Item& item, World& targetWorld) {
	TextBuffer.push("엔더진주를 사용할 방향을 입력하세요.");
	TextBuffer.push("( 플레이어 위치를 기준으로, 상:w, 좌:a, 하:s, 우:d, 취소:x )");
	targetWorld.printWorld();
	printf("입력 : ");
	char Direction; std::cin >> Direction;

	int tempR = targetWorld.player.Pos[targetWorld.worldType].first, tempC = targetWorld.player.Pos[targetWorld.worldType].second;

	switch (Direction) {
	case 'W':
	case 'w':
		while (tempR >= 0 && targetWorld.world[targetWorld.worldType][tempR][tempC].itemID <= 0) {
			tempR--;
		}
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = 0;
		targetWorld.player.Pos[targetWorld.worldType].first = tempR + 1;
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = -1;
		break;
	case 'A':
	case 'a':
		while (tempC >= 0 && targetWorld.world[targetWorld.worldType][tempR][tempC].itemID <= 0) {
			tempC--;
		}
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = 0;
		targetWorld.player.Pos[targetWorld.worldType].second = tempC + 1;
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = -1;
		break;
	case 'S':
	case 's':
		while (tempR < WORLD_HEIGHT && targetWorld.world[targetWorld.worldType][tempR][tempC].itemID <= 0) {
			tempR++;
		}
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = 0;
		targetWorld.player.Pos[targetWorld.worldType].first = tempR - 1;
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = -1;
		break;
	case 'D':
	case 'd':
		while (tempC < WORLD_WIDTH && targetWorld.world[targetWorld.worldType][tempR][tempC].itemID <= 0) {
			tempC++;
		}
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = 0;
		targetWorld.player.Pos[targetWorld.worldType].second = tempC - 1;
		targetWorld.world[targetWorld.worldType][targetWorld.player.Pos[targetWorld.worldType].first][targetWorld.player.Pos[targetWorld.worldType].second].itemID = -1;
		break;
	}
	TextBuffer.push("엔더진주를 사용해 순간이동 했습니다!");
}
void battleUseItem368(Player& player, Monster& enemy) {
	return; // Battle.cpp 에 구현되어 있음
}

void useItem381(Item& item, World& targetWorld) {
	Player& Player = targetWorld.player;
	Player.getItem(381, 1);

	if (Player.Inventory[381].count > 12) {
		Player.loseItem(381, 12);
		targetWorld.worldType = THE_END;
		TextBuffer.push("엔드로 이동했습니다!");
	}
	else {
		TextBuffer.push("엔더의 눈이 부족합니다.");
	}
}

void useItem423(Item& item, World& world) {
	TextBuffer.push("양고기를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(40)) + " 회복했습니다.");
}
void battleUseItem423(Player& player, Monster& enemy) {
	battleTextBuffer.push("양고기를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(40)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem424(Item& item, World& world) {
	TextBuffer.push("익힌 양고기를 먹었습니다.");
	TextBuffer.push("체력을 " + std::to_string(world.player.GetHeal(100)) + " 회복했습니다.");
}
void battleUseItem424(Player& player, Monster& enemy) {
	battleTextBuffer.push("익힌 양고기를 먹었다.");
	battleTextBuffer.push("체력을 " + std::to_string(player.GetHeal(100)) + " 회복했다!");
	if (player.BuffState[BLOOMING_CRYSTAL] > 0) {
		int CrystalDecrease = player.BuffState[BLOOMING_CRYSTAL] - range(player.BuffState[BLOOMING_CRYSTAL] - 3, 0, 7);
		battleTextBuffer.push("[ 싹트는 수정 ] 중첩이 " + std::to_string(CrystalDecrease) + "감소했다!");
		player.BuffState[BLOOMING_CRYSTAL] -= CrystalDecrease;
	}
}

void useItem1000(Item& item, World& world) {
	world.tryBattle(10000);
}

void useItem1001(Item& item, World& world) {
	std::system("cls");
	Player& player = world.player;
	WorldType worldType = world.worldType;
	player.getItem(1001, 1);

	std::vector<std::string> typeList = { "검류", "활류", "장식", };
	printf("\n");
	printUILine((CRAFT_UI_WIDTH - 14) / 2, 1, 0, false);
	printf("[======== 조합 아이템 목록 ========]");
	printUILine(0, 1, (CRAFT_UI_WIDTH - 14) / 2, true);
	printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);

	for (int i = 0; i <= 2; i++) {
		std::queue<std::string> craftPrintBuffer;
		std::string craftTempText = "";

		printUILine(1, CRAFT_UI_WIDTH / 2 - 4, 0, false);
		printf("[ ==== ");
		printf("%s", typeList[i].c_str());
		printf(" ==== ]");
		printUILine(0, CRAFT_UI_WIDTH / 2 - 4, 1, true);

		// 타입에 맞는 각 아이템 출력
		for (const auto& recipe : RECIPEBOOK) {
			std::vector<std::pair<int, int>> meterials = recipe.first;
			std::pair<int, int> result = recipe.second;
			if (result.first < 3000) {
				continue;
			}
			if (ITEM_DATABASE[result.first].type != i + 7) {
				continue;
			}

			craftTempText.clear();
			bool IsCraftable = true;
			for (std::pair<int, int> meterial : meterials) {
				if (player.Inventory[meterial.first].count < meterial.second) {
					IsCraftable = false;
				}

				if (meterial.second > 0) {
					craftTempText += ITEM_DATABASE[meterial.first].name + " " + std::to_string(meterial.second) + "개";
					craftTempText += " + ";
				}
				else {
					craftTempText.pop_back();
					craftTempText.pop_back();
					craftTempText += "(" + ITEM_DATABASE[meterial.first].name + " 필요)   ";
				}
			}
			craftTempText.pop_back();
			craftTempText.pop_back();
			craftTempText += "--> ";
			craftTempText += ITEM_DATABASE[result.first].name + " " + std::to_string(result.second) + "개";

			while (getTextWidth(craftTempText) < (3 * CRAFT_UI_WIDTH - 12) / 2) {
				craftTempText += " ";
			}

			if (IsCraftable) {
				craftPrintBuffer.push(ColorText({ 0, 255, 255 }, craftTempText));
			}
			else {
				craftPrintBuffer.push(ColorText({ 160, 160, 160 }, craftTempText));
			}


		}
		int totalRow = (static_cast<int>(craftPrintBuffer.size()) + 1) / 2;

		for (int row = 0; row < totalRow; row++) {
			if (craftPrintBuffer.empty()) {
				break;
			}
			printUILine(0, 2, 0, false);
			printf("%s", craftPrintBuffer.front().c_str());
			craftPrintBuffer.pop();
			if (craftPrintBuffer.empty()) {
				printf("\n");
				break;
			}
			printf("%s\n", craftPrintBuffer.front().c_str());
			craftPrintBuffer.pop();
		}
		printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);
		printUILine(CRAFT_UI_WIDTH, 0, 0, true);
		if (i < 2) {
			printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);
		}
	}

	printf("\n조합하고자 하는 아이템의 이름을 입력하세요(취소하려면 \"X\"를 입력) : ");
	std::string Input; Input = InputString();
	if (Input == "X" || Input == "x")
		return;

	bool IsItemExist = false;
	bool IsCraftable = true;
	bool IsNearTable = true;
	int MaxCraftCount = 65535;
	std::vector<std::pair<int, int>> TargetMeterials;
	std::pair<int, int> TargetResult;

	for (const auto& recipe : RECIPEBOOK) {
		std::vector<std::pair<int, int>> meterials = recipe.first;
		std::pair<int, int> result = recipe.second;
		if (result.first < 3000) {
			continue;
		}

		if (ITEM_DATABASE[result.first].name == Input) {
			IsItemExist = true;
			TargetMeterials = meterials;
			TargetResult = result;
			for (std::pair<int, int> meterial : meterials) {
				if (meterial.second > 0) {
					if (player.Inventory[meterial.first].count < meterial.second) {
						IsCraftable = false;
						break;
					}
					MaxCraftCount = min(MaxCraftCount, player.Inventory[meterial.first].count / meterial.second);
				}
			}
		}
	}

	if (!IsItemExist) {
		TextBuffer.push("존재하지 않는 아이템 이름입니다.");
		return;
	}
	if (!IsCraftable) {
		TextBuffer.push("재료가 부족하여 제작할 수 없는 아이템입니다.");
		return;
	}
	if (!IsNearTable) {
		TextBuffer.push("적절한 작업 블럭이 근처에 없어 제작할 수 없습니다.");
		return;
	}

	// 2-1. 조합 횟수 입력 받기
	printf("조합 횟수를 정수로 입력하세요. (최대 조합 가능 횟수 : %d회) : ", MaxCraftCount);
	int CraftCount; std::cin >> CraftCount;
	if (CraftCount > MaxCraftCount) {
		TextBuffer.push("조합할 수 없는 수량입니다.");
		return;
	}

	// 3. 가능한 아이템 입력한 경우 조합 처리
	std::string CraftText = "";

	for (std::pair<int, int> meterial : TargetMeterials) {
		if (meterial.second > 0) {
			player.loseItem(meterial.first, CraftCount * meterial.second);
			CraftText += ITEM_DATABASE[meterial.first].name + " " + std::to_string(CraftCount * meterial.second) + "개, ";
		}
	}
	CraftText.pop_back();
	CraftText.pop_back();
	CraftText += "를 사용했습니다.";
	TextBuffer.push(CraftText);
	CraftText.clear();
	CraftText = ITEM_DATABASE[TargetResult.first].name + " " + std::to_string(CraftCount * TargetResult.second) + "개를 제작했습니다.";
	TextBuffer.push(CraftText);

	if (TargetResult.first == 3000 && player.WeaponPower < 80) {
		player.WeaponPower = 80;
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (TargetResult.first == 3001 && (player.Equipment & (long long)PlayerEquipment::BOW3001) == 0) {
		player.Equipment |= (long long)PlayerEquipment::BOW3001;
		TextBuffer.push("전투에서 각 공격마다 추가 피해를 입힙니다.");
	}
	if (TargetResult.first == 3002 && (player.Equipment & (long long)PlayerEquipment::ACC3002) == 0) {
		player.Equipment |= (long long)PlayerEquipment::ACC3002;
		TextBuffer.push("전투에서 각 공격이 불타오름 효과를 적용시킵니다.");
	}
	if (TargetResult.first == 3003 && (player.Equipment & (long long)PlayerEquipment::ACC3003) == 0) {
		player.Equipment |= (long long)PlayerEquipment::ACC3003;
		TextBuffer.push("전투에서 피해를 받으면 피해의 일부를 회복합니다.");
	}

	player.getItem(TargetResult.first, CraftCount * TargetResult.second);
}

void battleUseItem3001(Player& player, Monster& enemy) {
	player.getItem(3001, 1);
	if (player.Inventory[262].count <= 0) {
		battleTextBuffer.push("화살이 없어 활로 " + enemy.name + "를 후려쳤다.");
		int damage = (player.AttackPower + player.WeaponPower) * 8 / 10;
		battleTextBuffer.push("플레이어는 " + enemy.name + "에게 " + std::to_string(enemy.GetDamage(damage, 0)) + " 의 피해를 입혔다!");
		return;
	}
	player.loseItem(262, 1);

	if (enemy.entityID == 58) {
		battleTextBuffer.push("플레이어는 활을 쏘았으나, 엔더맨은 회피했다.");
		enemy.GetDamage(0, 0);
		return;
	}

	if (enemy.MaxHealth == enemy.Health) {
		battleTextBuffer.push("플레이어는 망령의 활을 사용하여 강력한 원거리 저격을 쏘았다!");
		int damage = (player.AttackPower + player.WeaponPower) * 4;
		battleTextBuffer.push("플레이어는 " + enemy.name + "에게 " + std::to_string(enemy.GetDamage(damage, 0)) + " 의 피해를 입혔다!");
	}
	else {
		battleTextBuffer.push("플레이어는 망령의 활을 사용하여 화살을 쏘았다.");
		int damage = (player.AttackPower + player.WeaponPower) * 2;
		battleTextBuffer.push("플레이어는 " + enemy.name + "에게 " + std::to_string(enemy.GetDamage(damage, 0)) + " 의 피해를 입혔다!");
	}
}
