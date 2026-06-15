#include "World.h"
#include "Block.h"
#include "BlockData.h"
#include "Basic.h"
#include "random.h"
#include "itemData.h"
#include "colorprint.h"
#include "battle.h"
#include "item.h"
#include <iostream>
#include <vector>
#include <string>

void World::SwitchItem() {


	// 2. 아이템 이름 입력받기
	TextBuffer.push("손에 들고자 하는 아이템의 이름을 입력하세요.");
	TextBuffer.push("(취소하려면 \"X\"를 입력) : ");
	printWorld();

	// 1. 보유한 아이템 출력하기
	player.ShowInventory();

	printf("입력 : ");
	//std::string Input; std::cin.ignore(); std::getline(std::cin, Input);
	std::string Input; Input = InputString();
	if (Input == "X" || Input == "x")
		return;

	// 3. 손에 든 아이템 설정하기
	if (ItemName2ItemID[Input] == 0 || player.Inventory[ItemName2ItemID[Input]].count <= 0) {
		TextBuffer.push("보유하지 않은 아이템이거나 올바른 입력이 아닙니다.");
		return;
	}
	player.HandItemID = ItemName2ItemID[Input];
	TempTextLine = "손에 [" + Input + "] 아이템을 들었습니다.";
	TextBuffer.push(TempTextLine);
}

void World::PlaceBlock() {
	// 1. 손에 든 아이템이 블럭인지 확인하기
	if (player.HandItemID < 1 || player.HandItemID > 255) {
		TextBuffer.push("현재 손에 든 아이템은 설치할 수 있는 아이템이 아닙니다.");
		return;
	}

	// 2. 설치할 방향 입력받기
	TextBuffer.push("블럭을 설치할 위치를 입력하세요.");
	TextBuffer.push("( 플레이어 위치를 기준으로, 상:w, 좌:a, 하:s, 우:d, 취소:x )");
	printWorld();
	printf("입력 : ");
	char Direction; std::cin >> Direction;

	// 3. 해당 위치가 블럭 설치가 가능한지 확인하기
	int targetR = 0, targetC = 0;
	switch (Direction) {
	case 'w':
		targetR = player.Pos[worldType].first - 1;
		targetC = player.Pos[worldType].second;
		break;
	case 's':
		targetR = player.Pos[worldType].first + 1;
		targetC = player.Pos[worldType].second;
		break;
	case 'a':
		targetR = player.Pos[worldType].first;
		targetC = player.Pos[worldType].second - 1;
		break;
	case 'd':
		targetR = player.Pos[worldType].first;
		targetC = player.Pos[worldType].second + 1;
		break;
	case 'x':
	case 'X':
		return;
	}
	if (world[worldType][targetR][targetC].itemID != 0) {
		TextBuffer.push("해당 위치에는 블럭을 설치할 수 없습니다.");
		return;
	}

	// 4. 월드 해당 위치에 블럭 생성하기
	world[worldType][targetR][targetC].itemID = player.HandItemID;
	world[worldType][targetR][targetC].ColorNoise[0] = RandomIntRange(-10, 10);
	world[worldType][targetR][targetC].ColorNoise[1] = RandomIntRange(-10, 10);
	world[worldType][targetR][targetC].ColorNoise[2] = RandomIntRange(-10, 10);

	// 5. 플레이어 인벤토리에서 블럭 1개 제거하기
	player.loseItem(player.HandItemID, 1);
	if (player.Inventory[player.HandItemID].count == 0) {
		player.HandItemID = 0;
	}

	TextBuffer.push(ITEM_DATABASE[world[worldType][targetR][targetC].itemID].name + " 1개를 설치하였습니다.");
	TextBuffer.push("현재 " + ITEM_DATABASE[world[worldType][targetR][targetC].itemID].name + " 보유 개수 : " + std::to_string(player.Inventory[world[worldType][targetR][targetC].itemID].count) + "개");
	currentTime = (currentTime + 1) % 180;
}

void World::printCraft() {
	// 1. 조합법 모두 표시, 가능한 조합법은 밝은 색으로 표시

	std::vector<std::string> typeList = { "무기", "도구", "블럭", "음식", "재료", "특수" };
	printf("\n");
	printUILine((CRAFT_UI_WIDTH - 14) / 2, 1, 0, false);
	printf("[======== 조합 아이템 목록 ========]");
	printUILine(0, 1, (CRAFT_UI_WIDTH - 14) / 2, true);
	printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);

	for (int i = 0; i <= 5; i++) {
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
			if (ITEM_DATABASE[result.first].type != i) {
				continue;
			}

			craftTempText.clear();
			bool IsCraftable = true;
			for (std::pair<int, int> meterial : meterials) {
				if (player.Inventory[meterial.first].count < meterial.second) {
					IsCraftable = false;
				}
				if (meterial.second < 0) {
					if (
						(world[worldType][range(player.Pos[worldType].first - 1, 0, WORLD_HEIGHT)][player.Pos[worldType].second].itemID != meterial.first) &&
						(world[worldType][range(player.Pos[worldType].first + 1, 0, WORLD_HEIGHT)][player.Pos[worldType].second].itemID != meterial.first) &&
						(world[worldType][player.Pos[worldType].first][range(player.Pos[worldType].second + 1, 0, WORLD_HEIGHT)].itemID != meterial.first) &&
						(world[worldType][player.Pos[worldType].first][range(player.Pos[worldType].second - 1, 0, WORLD_HEIGHT)].itemID != meterial.first)
						) {
						IsCraftable = false;
					}
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
		if (i < 5) {
			printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);
		}
	}
}

void World::craft() {
	// 1. 조합 창 출력
	std::system("cls");
	printCraft();

	// 2. 사용자에게 "X" 또는 아이템 이름 입력
	printf("조합하고자 하는 아이템의 이름을 입력하세요(취소하려면 \"X\"를 입력) : ");
	//std::string Input; std::cin.ignore(); std::getline(std::cin, Input);
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
				else {
					if (
						(world[worldType][range(player.Pos[worldType].first - 1, 0, WORLD_HEIGHT)][player.Pos[worldType].second].itemID != meterial.first) &&
						(world[worldType][range(player.Pos[worldType].first + 1, 0, WORLD_HEIGHT)][player.Pos[worldType].second].itemID != meterial.first) &&
						(world[worldType][player.Pos[worldType].first][range(player.Pos[worldType].second + 1, 0, WORLD_HEIGHT)].itemID != meterial.first) &&
						(world[worldType][player.Pos[worldType].first][range(player.Pos[worldType].second - 1, 0, WORLD_HEIGHT)].itemID != meterial.first)
						) {
						IsNearTable = false;
						break;
					}
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
	if (ItemName2ItemID[Input] >= 3000) {
		TextBuffer.push("해당 작업대에서 제작할 수 있는 아이템이 아닙니다.");
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
			player.loseItem(meterial.first, CraftCount* meterial.second);
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
	if (TargetResult.first == 270 && player.MiningPower < 1) {
		player.MiningPower = 1;
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다!");
	}
	if (TargetResult.first == 274 && player.MiningPower < 2) {
		player.MiningPower = 2;
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다!");
	}
	if (TargetResult.first == 257 && player.MiningPower < 3) {
		player.MiningPower = 3;
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다!");
	}
	if (TargetResult.first == 278 && player.MiningPower < 4) {
		player.MiningPower = 4;
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다!");
	}

	if (TargetResult.first == 268 && player.WeaponPower < 15) {
		player.WeaponPower = 15;
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (TargetResult.first == 272 && player.WeaponPower < 20) {
		player.WeaponPower = 20;
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (TargetResult.first == 267 && player.WeaponPower < 25) {
		player.WeaponPower = 25;
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (TargetResult.first == 283 && player.WeaponPower < 30) {
		player.WeaponPower = 30;
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (TargetResult.first == 276 && player.WeaponPower < 40) {
		player.WeaponPower = 40;
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}

	if (TargetResult.first == 1004 && (player.Equipment & (long long)PlayerEquipment::SHIELD) == 0) {
		player.Equipment |= (long long)PlayerEquipment::SHIELD;
		player.DefensePower += 50;
		player.HasBlocked = 0;
		TextBuffer.push("방패를 착용하여 방어력이 상승했습니다.");
		TextBuffer.push("각 전투에서 적의 첫 공격을 방어할 수 있습니다!");
	}


	player.getItem(TargetResult.first, CraftCount* TargetResult.second);
}

void World::giveItem() {
	TempTextLine = "아이템 코드와 개수를 각각 정수로 입력하세요.";
	TextBuffer.push(TempTextLine);
	printWorld();
	printf("입력 : ");
	int itemID, itemCount; std::cin >> itemID >> itemCount;
	player.getItem(itemID, itemCount);
	TempTextLine = "플레이어에게 " + ITEM_DATABASE[itemID].name + " " + std::to_string(itemCount) + "개를 지급했습니다.";
	TextBuffer.push(TempTextLine);
	TempTextLine.clear();

	if (itemID == 270 && player.MiningPower < 1) {
		player.MiningPower = 1;
		TextBuffer.push("[ 나무 곡괭이 ] 를 획득하였습니다!");
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다.");
	}
	if (itemID == 274 && player.MiningPower < 2) {
		player.MiningPower = 2;
		TextBuffer.push("[ 돌 곡괭이 ] 를 획득하였습니다!");
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다.");
	}
	if (itemID == 257 && player.MiningPower < 3) {
		player.MiningPower = 3;
		TextBuffer.push("[ 철 곡괭이 ] 를 획득하였습니다!");
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다.");
	}
	if (itemID == 278 && player.MiningPower < 4) {
		player.MiningPower = 4;
		TextBuffer.push("[ 다이아몬드 곡괭이 ] 를 획득하였습니다!");
		TextBuffer.push("채광력이 상승하여 더 많은 블록을 캘 수 있습니다.");
	}
	if (itemID == 268 && player.WeaponPower < 15) {
		player.WeaponPower = 15;
		TextBuffer.push("[ 나무 검 ] 를 획득하였습니다!");
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (itemID == 272 && player.WeaponPower < 20) {
		player.WeaponPower = 20;
		TextBuffer.push("[ 돌 검 ] 를 획득하였습니다!");
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (itemID == 267 && player.WeaponPower < 25) {
		player.WeaponPower = 25;
		TextBuffer.push("[ 철 검 ] 를 획득하였습니다!");
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (itemID == 283 && player.WeaponPower < 30) {
		player.WeaponPower = 30;
		TextBuffer.push("[ 금 검 ] 를 획득하였습니다!");
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (itemID == 276 && player.WeaponPower < 40) {
		player.WeaponPower = 40;
		TextBuffer.push("[ 다이아몬드 검 ] 를 획득하였습니다!");
		TextBuffer.push("공격력이 상승하여 전투 시 더 많은 피해를 입힙니다.");
	}
	if (itemID == 1004 && (player.Equipment & (long long)PlayerEquipment::SHIELD) == 0) {
		player.Equipment |= (long long)PlayerEquipment::SHIELD;
		player.DefensePower += 50;
		player.HasBlocked = 0;
		TextBuffer.push("방패를 착용하여 방어력이 상승했습니다.");
		TextBuffer.push("각 전투에서 적의 첫 공격을 방어할 수 있습니다!");
	}
	if (itemID == 3001 && (player.Equipment & (long long)PlayerEquipment::BOW3001) == 0) {
		player.Equipment |= (long long)PlayerEquipment::BOW3001;
		TextBuffer.push("전투에서 각 공격마다 추가 피해를 입힙니다.");
	}
	if (itemID == 3002 && (player.Equipment & (long long)PlayerEquipment::ACC3002) == 0) {
		player.Equipment |= (long long)PlayerEquipment::ACC3002;
		TextBuffer.push("전투에서 각 공격이 불타오름 효과를 적용시킵니다.");
	}
	if (itemID == 3003 && (player.Equipment & (long long)PlayerEquipment::ACC3003) == 0) {
		player.Equipment |= (long long)PlayerEquipment::ACC3003;
		TextBuffer.push("전투에서 피해를 받으면 피해의 일부를 회복합니다.");
	}
}

void World::printWorld() {
	std::system("cls");
	for (int row = 0; row < 2 * LENS_HEIGHT_SIZE + 5; row++) {
		if (row == 0 || row == 2 * LENS_HEIGHT_SIZE + 4) {
			for (int column = 0; column < 2 * LENS_WIDTH_SIZE + 4; column++) {
				PrintWithColor({ 128, 128, 100 }, { 128, 128, 100 }, "   ");
			}
			for (int column = 0; column < INTERVAL_MAP_WITH_CONSOLE; column++) {
				printf("   ");
			}
			printUILine(3 * LENS_WIDTH_SIZE + 4, 0, 0, true);
		}
		else if (row == 1 || row == 2 * LENS_HEIGHT_SIZE + 3) {
			PrintWithColor({ 128, 128, 100 }, { 128, 128, 100 }, "   ");
			for (int column = 0; column < 2 * LENS_WIDTH_SIZE + 2; column++) {
				printf("   ");
			}
			PrintWithColor({ 128, 128, 100 }, { 128, 128, 100 }, "   ");

			for (int column = 0; column < INTERVAL_MAP_WITH_CONSOLE; column++) {
				printf("   ");
			}
			printUILine(1, 3 * LENS_WIDTH_SIZE + 2, 1, true);
		}
		else {
			PrintWithColor({ 128, 128, 100 }, { 128, 128, 100 }, "   ");
			printf("   ");
			int tempRow = row - 2 + player.Pos[worldType].first - LENS_HEIGHT_SIZE;
			for (int tempColumn = player.Pos[worldType].second - LENS_WIDTH_SIZE; tempColumn < player.Pos[worldType].second + LENS_WIDTH_SIZE; tempColumn++) {
				if (tempRow < 0 || tempRow >= WORLD_HEIGHT || tempColumn < 0 || tempColumn >= WORLD_WIDTH) {
					printf("   ");
					continue;
				}
				world[worldType][max(min(tempRow, WORLD_HEIGHT - 1), 0)][max(min(tempColumn, WORLD_WIDTH - 1), 0)].printBlock(*this);
			}
			printf("   ");
			PrintWithColor({ 128, 128, 100 }, { 128, 128, 100 }, "   ");

			for (int column = 0; column < INTERVAL_MAP_WITH_CONSOLE; column++) {
				printf("   ");
			}
			printf("   ");
			if (row % 2 == 0 && !TextBuffer.empty()) {
				printf("   ");
				std::string TempText = TextBuffer.front(); TextBuffer.pop();
				while (TempText.length() < 6 * LENS_WIDTH_SIZE + 3) {
					TempText += " ";
				}
				std::cout << TempText;
			}
			else {
				for (int column = 0; column < 2 * LENS_WIDTH_SIZE + 2; column++) {
					printf("   ");
				}
			}
			printf("   ");
			printf("\n");

		}
	}
	printf("\n");
}

bool World::playerMine(int worldPosR, int worldPosC) {
	if (world[worldType][worldPosR][worldPosC].itemID != 0) {

		if (world[worldType][worldPosR][worldPosC].getHardness() > player.MiningPower) {
			TempTextLine = ColorText({ 225, 90, 90 }, "채광력이 부족하여 해당 블럭을 부술 수 없습니다.");
			TextBuffer.push(TempTextLine);
			TempTextLine = ColorText({ 225, 90, 90 }, std::to_string(world[worldType][worldPosR][worldPosC].getHardness()) + " 이상의 채광력이 필요합니다.");
			TextBuffer.push(TempTextLine);
			return false;
		}
		int gotItemID = world[worldType][worldPosR][worldPosC].getItem();

		// 아이템이 자갈인 경우, 특수 케이스
		if (world[worldType][worldPosR][worldPosC].itemID == 13) {
			int RandomValue = RandomIntRange(1, 30);
			if (RandomValue == 1) {
				gotItemID = 328;
			}
		}

		if (gotItemID != -1) {
			player.getItem(gotItemID, 1);
			// 채광 완료 및 현재 보유 개수 출력 함수
			TempTextLine = "[" + ITEM_DATABASE[gotItemID].name + "] 을(를) 획득했습니다!";
			TextBuffer.push(TempTextLine);
			TempTextLine = "현재 " + ITEM_DATABASE[gotItemID].name + " 보유 개수 : " + std::to_string(player.Inventory[gotItemID].count) + "개";
			TextBuffer.push(TempTextLine);
			TempTextLine.clear();
		}
		world[worldType][worldPosR][worldPosC].itemID = 0;
	}
	return true;
}

void World::playerMove(char direction) {
	int tempPosR = player.Pos[worldType].first, tempPosC = player.Pos[worldType].second;

	switch (direction) {
	case 'w':
		tempPosR = max(0, player.Pos[worldType].first - 1);
		break;
	case 'a':
		tempPosC = max(0, player.Pos[worldType].second - 1);
		break;
	case 's':
		tempPosR = min(WORLD_HEIGHT - 1, player.Pos[worldType].first + 1);
		break;
	case 'd':
		tempPosC = min(WORLD_WIDTH - 1, player.Pos[worldType].second + 1);
		break;
	}

	// 특이 케이스 : 이동한 블럭이 포탈일 경우
	if (world[worldType][tempPosR][tempPosC].itemID == 90) {
		if (this->worldType == OVERWORLD) {
			TextBuffer.push("네더 월드로 이동했습니다!");
			this->worldType = NETHER;
		}
		else if (this->worldType == NETHER) {
			TextBuffer.push("오버 월드로 이동했습니다!");
			this->worldType = OVERWORLD;
		}
	}


	// 특이 케이스 : 이동한 블럭이 '엔더 드래곤' 일 경우
	else if (world[worldType][tempPosR][tempPosC].itemID == 2006) {
		if (this->worldType == OVERWORLD) {
			TextBuffer.push("네더 월드로 이동했습니다!");
			this->worldType = NETHER;
		}
		else if (this->worldType == NETHER) {
			TextBuffer.push("오버 월드로 이동했습니다!");
			this->worldType = OVERWORLD;
		}
		std::system("cls");
		printUILine(CRAFT_UI_WIDTH, 0, 0, true);
		printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);
		printUILine(0, 2, 0, false);
		printf("[ 최종 전투 - 엔더 드래곤 ]\n\n");
		printUILine(0, 2, 0, false);
		printf("엔더 월드를 지배하는 거대한 용이다. 압도적인 힘으로 침입자를 막아선다.\n\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,255, 100 }, "체력 : 2500, 공격력 : 70, 방어력 : 100\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,255, 100 }, "특수 능력 - 엔더 드래곤의 모든 공격은 플레이어에게 [싹트는 수정] 디버프를 1중첩 적용합니다.\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,255, 100 }, "50% 확률로 1중첩을 추가로 부여합니다.\n\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,100, 255 }, "[ 싹트는 수정 ]\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,100, 255 }, "최대 7 중첩까지 중첩됩니다.\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,100, 255 }, "매 턴 마다 중첩당 5의 피해를 입습니다.\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,100, 255 }, "7중첩인 경우 대신 최대 체력의 300% 의 고정 피해를 입습니다.\n");
		printUILine(0, 2, 0, false);
		PrintWithColor({ 255 ,100, 255 }, "음식을 먹으면 [ 싹트는 수정 ] 의 3 중첩을 감소시킵니다.\n");

		printUILine(0, 2, 0, false);
		printf("충분한 준비를 마치고 전투를 시작하세요.\n");
		printUILine(0, 2, 0, false);
		printf("[y] 전투 시작 / [x] 취소\n");
		printUILine(1, CRAFT_UI_WIDTH - 2, 1, true);
		printUILine(CRAFT_UI_WIDTH, 0, 0, true);
		char Input = '0';
		while (true) {
			printf("입력 : ");
			std::cin >> Input;
			if (Input == 'Y' || Input == 'y' || Input == 'X' || Input == 'x') {
				break;
			}
		}
		if (Input == 'Y' || Input == 'y') {
			bool BossFightResult = battle(player, 63);
			if (!BossFightResult) {
				player.clearInventory(*this);
			}
			else {
				std::system("cls");
				End();
			}
		}
	}

	else if (playerMine(tempPosR, tempPosC)) {
		world[worldType][player.Pos[worldType].first][player.Pos[worldType].second].itemID = 0;
		world[worldType][tempPosR][tempPosC].itemID = -1;
		player.Pos[worldType].first = tempPosR;
		player.Pos[worldType].second = tempPosC;
		currentTime = (currentTime + 1) % 180;
		updateLiquid();
		if (!tryBattle()) {
			player.clearInventory(*this);
		};
	}

	EffectDebuff(player);
	if (player.Health <= 0) {
		player.clearInventory(*this);
	}

	if (IsPlayerNearbyBlock(10)) {
		TextBuffer.push("플레이어가 용암에 닿아 불타기 시작했습니다!");
		ApplyDebuff(player, FLAME, 5);
	}

	if (player.BuffState[FLAME] > 0 && IsPlayerNearbyBlock(8)) {
		TextBuffer.push("플레이어에 붙어 있던 화염이 물에 닿아 꺼졌습니다.");
		player.BuffState[FLAME] = 0;
	}
}
//낮 : t <= 120 or t >= 170
//밤 : 120 < t < 170

bool World::tryBattle(int Chance) {
	bool Result = true;
	if (player.doesNotMeetMonster || Chance == 0) {
		return true;
	}
	if (worldType == OVERWORLD) {
		if (player.Pos[worldType].first <= 12 && (currentTime <= 120 || currentTime >= 170)) {
			int randomValue = rand() % (3 * 10000 / Chance);

			// 0.5% = 돼지
			if (randomValue == 0) {
				Result = battle(player, 90);
				player.ChanceMeetMonster = 80;
			}
			// 0.5% = 양
			else if (randomValue == 1) {
				Result = battle(player, 91);
				player.ChanceMeetMonster = 80;
			}
			// 0.5% = 소
			else if (randomValue == 2) {
				Result = battle(player, 92);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
		else if (player.Pos[worldType].first > 12 || (currentTime > 120 && currentTime < 170)) {

			int randomValue = rand() % (5 * 10000 / Chance);

			// 0.3% = 크리퍼
			if (randomValue == 0) {
				Result = battle(player, 50);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 스켈레톤
			else if (randomValue == 1) {
				Result = battle(player, 51);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 거미
			else if (randomValue == 2) {
				Result = battle(player, 52);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 좀비
			else if (randomValue == 3) {
				Result = battle(player, 54);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 엔더맨
			else if (randomValue == 4) {
				Result = battle(player, 58);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
	}
	else if (worldType == NETHER) {
		if (player.Pos[NETHER].second <= WORLD_WIDTH / 3) {
			int randomValue = rand() % (1 * 10000 / Chance);
			if (randomValue == 0) {
				// 엔더맨
				Result = battle(player, 58);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
		else if (player.Pos[NETHER].second >= 3 * WORLD_WIDTH / 4) {
			int randomValue = rand() % (2 * 10000 / Chance);
			if (randomValue == 0) {
				// 위더 스켈레톤
				Result = battle(player, 5);
				player.ChanceMeetMonster = 80;
			}
			else if (randomValue == 1) {
				// 블레이즈
				Result = battle(player, 61);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
		else {
			int randomValue = rand() % (3 * 10000 / Chance);
			if (randomValue == 0) {
				// 스켈레톤
				Result = battle(player, 51);
				player.ChanceMeetMonster = 80;
			}
			else if (randomValue == 1) {
				// 가스트
				Result = battle(player, 56);
				player.ChanceMeetMonster = 80;
			}
			else if (randomValue == 2) {
				// 마그마 크림
				Result = battle(player, 62);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
	}
	else if (worldType == THE_END) {
		int RandomValue = rand() % (10000 / Chance);
		if (RandomValue == 0) {
			// 엔더맨
			Result = battle(player, 58);
			player.ChanceMeetMonster = 80;
		}
		else {
			player.ChanceMeetMonster += 2;
		}
	}
	return Result;
}

bool World::tryBattle() {
	int Chance = player.ChanceMeetMonster;
	bool Result = true;
	if (player.doesNotMeetMonster || Chance == 0) {
		return true;
	}
	if (worldType == OVERWORLD) {
		if (player.Pos[worldType].first <= 12 && (currentTime <= 120 || currentTime >= 170)) {
			int randomValue = rand() % (3 * 10000 / Chance);

			// 0.5% = 돼지
			if (randomValue == 0) {
				Result = battle(player, 90);
				player.ChanceMeetMonster = 80;
			}
			// 0.5% = 양
			else if (randomValue == 1) {
				Result = battle(player, 91);
				player.ChanceMeetMonster = 80;
			}
			// 0.5% = 소
			else if (randomValue == 2) {
				Result = battle(player, 92);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
		else if (player.Pos[worldType].first > 12 || (currentTime > 120 && currentTime < 170)) {
		
			int randomValue = rand() % (5 * 10000 / Chance);

			// 0.3% = 크리퍼
			if (randomValue == 0) {
				Result = battle(player, 50);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 스켈레톤
			else if (randomValue == 1) {
				Result = battle(player, 51);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 거미
			else if (randomValue == 2) {
				Result = battle(player, 52);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 좀비
			else if (randomValue == 3) {
				Result = battle(player, 54);
				player.ChanceMeetMonster = 80;
			}
			// 0.3% = 엔더맨
			else if (randomValue == 4) {
				Result = battle(player, 58);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
	}
	else if (worldType == NETHER) {
		if (player.Pos[NETHER].second <= WORLD_WIDTH / 3) {
			int randomValue = rand() % (1 * 10000 / Chance);
			if (randomValue == 0) {
				// 엔더맨
				Result = battle(player, 58);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
		else if (player.Pos[NETHER].second >= 3 * WORLD_WIDTH / 4) {
			int randomValue = rand() % (2 * 10000 / Chance);
			if (randomValue == 0) {
				// 위더 스켈레톤
				Result = battle(player, 5);
				player.ChanceMeetMonster = 80;
			}
			else if (randomValue == 1) {
				// 블레이즈
				Result = battle(player, 61);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
		else {
			int randomValue = rand() % (3 * 10000 / Chance);
			if (randomValue == 0) {
				// 스켈레톤
				Result = battle(player, 51);
				player.ChanceMeetMonster = 80;
			}
			else if (randomValue == 1) {
				// 가스트
				Result = battle(player, 56);
				player.ChanceMeetMonster = 80;
			}
			else if (randomValue == 2) {
				// 마그마 크림
				Result = battle(player, 62);
				player.ChanceMeetMonster = 80;
			}
			else {
				player.ChanceMeetMonster += 2;
			}
		}
	}
	else if (worldType == THE_END) {
		int RandomValue = rand() % (10000 / Chance);
		if (RandomValue == 0) {
			// 엔더맨
			Result = battle(player, 58);
			player.ChanceMeetMonster = 80;
		}
		else {
			player.ChanceMeetMonster += 2;
		}
	}
	return Result;
}

void World::useItemInWorld() {
	// 1. 손에 든 아이템이 사용 가능한 아이템인지 확인하기
	if (!player.Inventory[player.HandItemID].isUsable()) {
		TextBuffer.push("현재 손에 든 아이템은 사용할 수 있는 아이템이 아닙니다.");
		return;
	}

	const int& itemID = player.HandItemID;
	Item& targetItem = player.Inventory[player.HandItemID];

	// 2. 사용하기
	ITEM_DATABASE[itemID].useFunc(targetItem, *this);
	
	// 3. 아이템 개수 줄이기
	player.loseItem(player.HandItemID, 1);
	if (targetItem.count == 0) {
		player.HandItemID = 0;
	}
}

void World::getItemInfo() {
	TextBuffer.push("정보를 확인하려는 아이템의 이름을 정확히 입력하세요.");
	printWorld();
	printf("입력 : ");
	//std::string Input; std::cin.ignore(); std::getline(std::cin, Input);
	std::string Input; Input = InputString();
	if (ItemName2ItemID.find(Input) == ItemName2ItemID.end()) {
		TextBuffer.push("[ " + Input + " ] 는(은) 존재하지 않는 아이템 이름입니다.");
	}
	else {
		int itemID = ItemName2ItemID[Input];
		ITEM_DATABASE[itemID].infoFunc();
	}
}

void World::updateLiquid() {
	for (int r = WORLD_HEIGHT - 1; r >= 0; r--) {
		for (int c = 0; c < WORLD_WIDTH; c++) {
			if (BLOCK_DATABASE[world[worldType][r][c].itemID].solid) {
				continue;
			}
			// 액체 블럭이 좌우로 퍼질 조건 : 해당 블럭이 '해당 블럭과 같지 않은' 블럭으로 아래가 막혀있을 경우
			if ((r == WORLD_HEIGHT - 1) || ((world[worldType][r + 1][c].itemID != 0) && (world[worldType][r][c].itemID != world[worldType][r + 1][c].itemID))) {
				// 만일 다른 성질의 액체가 있으면 흑요석을 생성한다.
				if (c > 0 && !BLOCK_DATABASE[world[worldType][r][c - 1].itemID].solid && world[worldType][r][c].itemID != world[worldType][r][c - 1].itemID) {
					world[worldType][r][c - 1].itemID = 49;
				}
				else if (c > 0 && world[worldType][r][c - 1].itemID == 0) {
					world[worldType][r][c - 1].itemID = world[worldType][r][c].itemID;
				}

				if (c < WORLD_WIDTH - 1 && !BLOCK_DATABASE[world[worldType][r][c + 1].itemID].solid && world[worldType][r][c].itemID != world[worldType][r][c + 1].itemID) {
					world[worldType][r][c + 1].itemID = 49;
				}
				else if (c < WORLD_WIDTH - 1 && world[worldType][r][c + 1].itemID == 0) {
					world[worldType][r][c + 1].itemID = world[worldType][r][c].itemID;
				}
			}

			// 액체 블럭이 아래로 퍼질 조건 : 해당 블럭의 아래 부분이 막히지 않은 경우
			if (r < WORLD_HEIGHT - 1 && !BLOCK_DATABASE[world[worldType][r + 1][c].itemID].solid && world[worldType][r][c].itemID != world[worldType][r + 1][c].itemID) {
				world[worldType][r + 1][c].itemID = 49;
			}
			else if (r < WORLD_HEIGHT - 1 && (world[worldType][r + 1][c].itemID == 0)) {
				world[worldType][r + 1][c].itemID = world[worldType][r][c].itemID;
			}
		}
	}
}

bool World::IsPlayerNearbyBlock(int BlockID) {
	bool Result = false;

	if (!Result && (player.Pos[worldType].first - 1 >= 0) && (world[worldType][player.Pos[worldType].first - 1][player.Pos[worldType].second].itemID == BlockID)) {
		Result = true;
	}

	if (!Result && (player.Pos[worldType].first + 1 < WORLD_HEIGHT) && (world[worldType][player.Pos[worldType].first + 1][player.Pos[worldType].second].itemID == BlockID)) {
		Result = true;
	}

	if (!Result && (player.Pos[worldType].second - 1 >= 0) && (world[worldType][player.Pos[worldType].first][player.Pos[worldType].second - 1].itemID == BlockID)) {
		Result = true;
	}

	if (!Result && (player.Pos[worldType].second + 1 < WORLD_WIDTH) && (world[worldType][player.Pos[worldType].first][player.Pos[worldType].second + 1].itemID == BlockID)) {
		Result = true;
	}
	
	return Result;
}

void World::TP() {
	TempTextLine = "텔레포트 할 월드의 코드, R 좌표, C 좌표를 정수로 입력하세요.";
	TextBuffer.push(TempTextLine);
	printWorld();
	int Type, TargetR, TargetC; std::cin >> Type >> TargetR >> TargetC;
	worldType = (WorldType)Type;
	int CurrentPosR = player.Pos[Type].first;
	int CurrentPosC = player.Pos[Type].second;
	TargetR = range(TargetR, 0, WORLD_HEIGHT - 1);
	TargetC = range(TargetR, 0, WORLD_WIDTH - 1);

	world[Type][CurrentPosR][CurrentPosC].itemID = 0;
	player.Pos[Type] = { TargetR, TargetC };
	world[Type][CurrentPosR][CurrentPosC].itemID = -1;
	TempTextLine = "텔레포트 완료!";
	printWorld();
}
