#include "WorldGenerate.h"
#include "World.h"
#include "basic.h"
#include "random.h"
#include "Block.h"
#include <vector>
#include <queue>
#include <algorithm>

void WorldGenerator::WorldGenerate(World& targetWorld) {
	Player& player = targetWorld.player;

	targetWorld.world.push_back(std::vector<std::vector<Block>>(WORLD_HEIGHT, std::vector<Block>(WORLD_WIDTH, Block())));
	targetWorld.world.push_back(std::vector<std::vector<Block>>(WORLD_HEIGHT, std::vector<Block>(WORLD_WIDTH, Block())));
	targetWorld.world.push_back(std::vector<std::vector<Block>>(WORLD_HEIGHT, std::vector<Block>(WORLD_WIDTH, Block())));
	
	// [ 오버 월드 생성 ]
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {

			// 월드 기본 생성
			if (row <= 7) {
				targetWorld.world[OVERWORLD][row][column].itemID = 0;
			}
			else if (row == 8) {
				targetWorld.world[OVERWORLD][row][column].itemID = 2;
			}
			else if (row >= 9 && row <= 12) {
				targetWorld.world[OVERWORLD][row][column].itemID = 3;
			}
			else {
				targetWorld.world[OVERWORLD][row][column].itemID = 1;
			}
			targetWorld.world[OVERWORLD][row][column].posR = row;
			targetWorld.world[OVERWORLD][row][column].posC = column;
			targetWorld.world[OVERWORLD][row][column].ColorNoise[0] = RandomIntRange(-7, 7);
			targetWorld.world[OVERWORLD][row][column].ColorNoise[1] = RandomIntRange(-7, 7);
			targetWorld.world[OVERWORLD][row][column].ColorNoise[2] = RandomIntRange(-7, 7);
		}
	}

	// 광물 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 15) {
				// 석탄 생성
				if (RandomIntRange(1, 1000) >= 995) {
					PlantOre(targetWorld, OVERWORLD, row, column, 16);
				}
			}
			if (row >= 18) {
				// 철 생성
				if (RandomIntRange(1, 1000) >= 995) {
					PlantOre(targetWorld, OVERWORLD, row, column, 15);
				}
			}
			if (row >= 27) {
				// 철 생성
				if (RandomIntRange(1, 1000) >= 995) {
					PlantOre(targetWorld, OVERWORLD, row, column, 14);
				}
			}
			if (row >= 35) {
				// 다이아몬드 생성
				if (RandomIntRange(1, 1000) >= 993) {
					PlantOre(targetWorld, OVERWORLD, row, column, 56);
				}
			}
		}
	}

	// 자갈 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 17) {
				if (RandomIntRange(1, 3000) >= 2996) {
					PlantMass(targetWorld, OVERWORLD, row, column, 13, 70);
				}
			}
		}
	}

	// 구덩이 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 17) {
				if (RandomIntRange(1, 3000) >= 2980) {
					PlantMass(targetWorld, OVERWORLD, row, column, 0, 70);
				}
			}
		}
	}

	// 액체 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 20 && row <= 40) {
				if (RandomIntRange(1, 3000) >= 2995) {
					PlantMass(targetWorld, OVERWORLD, row, column, 8, 65);
				}
			}
			else if (row > 40) {
				if (RandomIntRange(1, 3000) >= 2995) {
					PlantMass(targetWorld, OVERWORLD, row, column, 10, 65);
				}
			}
		}
	}

	// 나무 생성
	std::vector<int> treePosList = RandomIntRange(0, WORLD_WIDTH - 1, RandomIntRange(3, 5));
	for (int treePos : treePosList) {
		PlantTree(targetWorld, OVERWORLD, 7, treePos, 17, 18);
	}

	// 기반암 생성
	for (int row = WORLD_HEIGHT - 4; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			int RandomValue = RandomIntRange(0, (WORLD_HEIGHT - row) - 1);
			if (RandomValue == 0) {
				targetWorld.world[OVERWORLD][row][column].itemID = 7;
			}
		}
	}

	// 플레이어 생성
	player.Pos[OVERWORLD].first = 7;
	player.Pos[OVERWORLD].second = WORLD_WIDTH / 2;
	targetWorld.world[OVERWORLD][player.Pos[OVERWORLD].first][player.Pos[OVERWORLD].second].itemID = -1;

	//// 테스트 블럭 생성
	/*targetWorld.world[OVERWORLD][player.Pos[OVERWORLD].first - 1][player.Pos[OVERWORLD].second].itemID = 90;
	targetWorld.world[OVERWORLD][player.Pos[OVERWORLD].first - 1][player.Pos[OVERWORLD].second + 1].itemID = 121;*/


	// [ 네더 월드 생성 ]
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {

			// 월드 기본 생성
			if (row < 8) {
				targetWorld.world[NETHER][row][column].itemID = 0;
			}
			else if (row == 8) {
				if (column <= WORLD_WIDTH / 3) {
					targetWorld.world[NETHER][row][column].itemID = 2001;
				}
				else if (column <= WORLD_WIDTH / 3 + 5) {
					if (RandomIntRange(0, 1) == 0) {
						targetWorld.world[NETHER][row][column].itemID = 2001;
					}
					else {
						targetWorld.world[NETHER][row][column].itemID = 87;
					}
				}
				else {
					targetWorld.world[NETHER][row][column].itemID = 87;
				}
			}
			else if (row > 8) {
				targetWorld.world[NETHER][row][column].itemID = 87;
			}
			targetWorld.world[NETHER][row][column].posR = row;
			targetWorld.world[NETHER][row][column].posC = column;
			targetWorld.world[NETHER][row][column].ColorNoise[0] = RandomIntRange(-7, 7);
			targetWorld.world[NETHER][row][column].ColorNoise[1] = RandomIntRange(-7, 7);
			targetWorld.world[NETHER][row][column].ColorNoise[2] = RandomIntRange(-7, 7);
		}
	}

	// 광물 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 15) {
				// 네더 금 광석
				if (RandomIntRange(1, 1000) >= 990) {
					PlantOre(targetWorld, NETHER, row, column, 2004);
				}
			}
			if (row >= 3 * WORLD_HEIGHT / 4) {
				// 고대 잔해
				if (RandomIntRange(1, 1000) >= 960) {
					PlantOre(targetWorld, NETHER, row, column, 2005);
				}
			}
		}
	}

	// 영혼 모래 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 14) {
				if (RandomIntRange(1, 3000) >= 2990) {
					PlantMass(targetWorld, NETHER, row, column, 88, 75);
				}
			}
		}
	}

	// 자갈 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 14) {
				if (RandomIntRange(1, 3000) >= 2990) {
					PlantMass(targetWorld, NETHER, row, column, 13, 70);
				}
			}
		}
	}

	// 흑암 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 14) {
				if (RandomIntRange(1, 3000) >= 2990) {
					PlantMass(targetWorld, NETHER, row, column, 2000, 70);
				}
			}
		}
	}

	// 발광석 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row <= 1) {
				if (RandomIntRange(1, 3000) >= 2930) {
					PlantMass(targetWorld, NETHER, row, column, 89, 65);
				}
			}
		}
	}

	// 구덩이 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 17) {
				if (RandomIntRange(1, 3000) >= 2980) {
					PlantMass(targetWorld, NETHER, row, column, 0, 70);
				}
			}
		}
	}

	// 용암 생성
	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			if (row >= 20) {
				if (RandomIntRange(1, 3000) >= 2990) {
					PlantMass(targetWorld, NETHER, row, column, 10, 80);
				}
			}
		}
	}

	// 나무 생성
	treePosList = RandomIntRange(0, WORLD_WIDTH / 3 - 1, RandomIntRange(2, 3));
	for (int treePos : treePosList) {
		PlantTree(targetWorld, NETHER, 7, treePos, 2002, 2003);
	}

	// 네더 유적 생성
	for (int row = 8; row < WORLD_HEIGHT; row++) {
		for (int column = WORLD_WIDTH - 7; column <= WORLD_WIDTH - 3; column++) {
			targetWorld.world[NETHER][row][column].itemID = 112;
		}
	}
	for (int column = 3 * WORLD_WIDTH / 4; column < WORLD_WIDTH; column++) {
		targetWorld.world[NETHER][8][column].itemID = 112;
	}
	for (int column = 3 * WORLD_WIDTH / 4; column < WORLD_WIDTH; column++) {
		targetWorld.world[NETHER][4][column].itemID = 112;
	}
	for (int column = 3 * WORLD_WIDTH / 4 + 2; column < WORLD_WIDTH; column++) {
		targetWorld.world[NETHER][3][column].itemID = 112;
	}
	for (int column = 3 * WORLD_WIDTH / 4 + 4; column < WORLD_WIDTH; column++) {
		targetWorld.world[NETHER][2][column].itemID = 112;
	}
	for (int column = 3 * WORLD_WIDTH / 4 + 6; column < WORLD_WIDTH; column++) {
		targetWorld.world[NETHER][1][column].itemID = 112;
	}

	// 네더 포탈 생성
	int tempPosR = 7;
	int tempPosC = WORLD_WIDTH / 2;

	tempPosC -= 2;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosR -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 3;
	tempPosR -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 3;
	tempPosR -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 3;
	tempPosR -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 90;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC -= 3;
	tempPosR -= 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;
	tempPosC += 1;
	targetWorld.world[NETHER][tempPosR][tempPosC].itemID = 49;

	// 플레이어 생성
	player.Pos[NETHER].first = 7;
	player.Pos[NETHER].second = WORLD_WIDTH / 2;
	targetWorld.world[NETHER][player.Pos[NETHER].first][player.Pos[NETHER].second].itemID = -1;

	// 기반암 생성
	for (int row = WORLD_HEIGHT - 4; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {
			int RandomValue = RandomIntRange(0, (WORLD_HEIGHT - row) - 1);
			if (RandomValue == 0) {
				targetWorld.world[NETHER][row][column].itemID = 7;
			}
		}
	}

	//// 테스트 블럭 생성
	//targetWorld.world[NETHER][player.Pos[NETHER].first][player.Pos[NETHER].second + 1].itemID = 90;

	// [ 엔드 생성 ]
	int CenterX = WORLD_WIDTH / 2;
	int CenterY = 31;

	for (int row = 0; row < WORLD_HEIGHT; row++) {
		for (int column = 0; column < WORLD_WIDTH; column++) {

			// 월드 기본 생성
			int X = column - CenterX;
			int Y = CenterY - row;

			if (Y > 0) {
				targetWorld.world[THE_END][row][column].itemID = 0;
			}
			else if (X * X + 3 * Y * Y > WORLD_WIDTH * WORLD_WIDTH / 8) {
				targetWorld.world[THE_END][row][column].itemID = 0;
			}
			else {
				targetWorld.world[THE_END][row][column].itemID = 121;
			}
			targetWorld.world[THE_END][row][column].posR = row;
			targetWorld.world[THE_END][row][column].posC = column;
			targetWorld.world[THE_END][row][column].ColorNoise[0] = RandomIntRange(-7, 7);
			targetWorld.world[THE_END][row][column].ColorNoise[1] = RandomIntRange(-7, 7);
			targetWorld.world[THE_END][row][column].ColorNoise[2] = RandomIntRange(-7, 7);
		}
	}

	// 흑요석 기둥 생성
	std::vector<int> ObsidianPillarPosList;
	bool HasObsidianPillarPlacedCorrectly = false;
	while (!HasObsidianPillarPlacedCorrectly) {
	    HasObsidianPillarPlacedCorrectly = true;

		ObsidianPillarPosList = RandomIntRange(WORLD_WIDTH / 4 + 1, WORLD_WIDTH * 3 / 4 - 1, 3);
		sort(ObsidianPillarPosList.begin(), ObsidianPillarPosList.end());

		if (ObsidianPillarPosList[1] - ObsidianPillarPosList[0] <= 4 || ObsidianPillarPosList[2] - ObsidianPillarPosList[1] <= 4) {
			HasObsidianPillarPlacedCorrectly = false;
		}

		if (abs(ObsidianPillarPosList[0] - WORLD_WIDTH / 2) < 2) {
			HasObsidianPillarPlacedCorrectly = false;
		}

		if (abs(ObsidianPillarPosList[1] - WORLD_WIDTH / 2) < 2) {
			HasObsidianPillarPlacedCorrectly = false;
		}

		if (abs(ObsidianPillarPosList[2] - WORLD_WIDTH / 2) < 2) {
			HasObsidianPillarPlacedCorrectly = false;
		}

		if (abs(ObsidianPillarPosList[0] - (WORLD_WIDTH / 2 + 5)) < 5) {
			HasObsidianPillarPlacedCorrectly = false;
		}

		if (abs(ObsidianPillarPosList[1] - (WORLD_WIDTH / 2 + 5)) < 5) {
			HasObsidianPillarPlacedCorrectly = false;
		}

		if (abs(ObsidianPillarPosList[2] - (WORLD_WIDTH / 2 + 5)) < 5) {
			HasObsidianPillarPlacedCorrectly = false;
		}
	}
	for (int ObsidianPillarPos : ObsidianPillarPosList) {
		int RandomHeight = RandomIntRange(14, 21);
		for (int row = 30; row >= 30 - RandomHeight; row--) {
			for (int column = ObsidianPillarPos - 1; column <= ObsidianPillarPos + 1; column++) {
				targetWorld.world[THE_END][row][column].itemID = 49;
			}
		}
	}

	// 기반암 제단 생성
	tempPosR = 30;
	tempPosC = WORLD_WIDTH / 2;

	tempPosC += 2;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosR++;
	tempPosC++;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosC++;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosC++;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosC++;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosC++;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosC++;
	tempPosR--;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosC -= 3;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosR--;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosR--;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosR--;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 7;
	tempPosR--;
	targetWorld.world[THE_END][tempPosR][tempPosC].itemID = 2006;

	// 플레이어 생성
	player.Pos[THE_END].first = 30;
	player.Pos[THE_END].second = WORLD_WIDTH / 2;
	targetWorld.world[THE_END][player.Pos[THE_END].first][player.Pos[THE_END].second].itemID = -1;
}

void WorldGenerator::PlantTree(World& targetWorld, WorldType worldType, int worldPosR, int worldPosC, int StemBlockID, int LeafBlockID) {

	std::vector<std::vector<Block>>& world = targetWorld.world[worldType];

	int StemHeight = RandomIntRange(3, 5);

	for (int h = 0; h < StemHeight; h++) {
		if (worldPosR - h < 0)
			break;
		world[worldPosR - h][worldPosC].itemID = StemBlockID;
	}
	for (int h = StemHeight - 2; h < StemHeight + 2; h++) {
		if (worldPosR - h < 0)
			break;
		if (h < StemHeight) {
			for (int c = -2; c <= 2; c++) {
				if (worldPosC + c < 0 || worldPosC + c >= WORLD_WIDTH)
					continue;
				if (c == 0)
					continue;
				world[worldPosR - h][worldPosC + c].itemID = LeafBlockID;
			}
		}
		else if (h == StemHeight) {
			for (int c = -2; c <= 2; c++) {
				if (worldPosC + c < 0 || worldPosC + c >= WORLD_WIDTH)
					continue;
				world[worldPosR - h][worldPosC + c].itemID = LeafBlockID;
			}
		}
		else {
			for (int c = -1; c <= 1; c++) {
				if (worldPosC + c < 0 || worldPosC + c >= WORLD_WIDTH)
					continue;
				world[worldPosR - h][worldPosC + c].itemID = LeafBlockID;
			}
		}
	}
}

#define SETBLOCK if (tempPosR >= 0 && tempPosR < WORLD_HEIGHT && tempPosC >= 0 && tempPosC < WORLD_WIDTH) {world[tempPosR][tempPosC].itemID = blockID;}
void WorldGenerator::PlantOre(World& targetWorld, WorldType worldType, int worldPosR, int worldPosC, int blockID) {
	std::vector<std::vector<Block>>& world = targetWorld.world[worldType];
	int randomValue = RandomIntRange(0, 6);

	int tempPosR = worldPosR;
	int tempPosC = worldPosC;
	
	switch (randomValue) {
	case 0:
		SETBLOCK
		tempPosC++;
		SETBLOCK
		break;
	case 1:
		SETBLOCK
		tempPosR++;
		SETBLOCK
		break;
	case 2:
		SETBLOCK
		tempPosC++;
		SETBLOCK
		tempPosC--; tempPosR++;
		SETBLOCK
		break;
	case 3:
		SETBLOCK
		tempPosR++;
		SETBLOCK
		tempPosC++;
		SETBLOCK
		break;
	case 4:
		SETBLOCK
		tempPosR++;
		SETBLOCK
		tempPosC--;
		SETBLOCK
		break;
	case 5:
		SETBLOCK
		tempPosC++;
		SETBLOCK
		tempPosR++;
		SETBLOCK
		break;
	case 6:
		SETBLOCK
		tempPosR++;
		SETBLOCK
		tempPosC++;
		SETBLOCK
		tempPosR--;
		SETBLOCK
		break;
	default:
		SETBLOCK
	}
}

void WorldGenerator::PlantMass(World& targetWorld, WorldType worldType, int worldPosR, int worldPosC, int blockID, int Size) {
	std::vector<std::vector<Block>>& world = targetWorld.world[worldType];

	Size = range(Size, 1, 100);
	int PercentDecreaseAmount = 100 - Size;
	
	std::vector<std::vector<bool>> Visited(WORLD_HEIGHT, std::vector<bool>(WORLD_WIDTH, false));
	std::vector<std::vector<int>> NumberTable(WORLD_HEIGHT, std::vector<int>(WORLD_WIDTH, 0));

	std::queue<std::pair<int, int>> Queue;
	Queue.push({worldPosR, worldPosC});
	NumberTable[worldPosR][worldPosC] = 100;

	while (!Queue.empty()) {
		std::pair<int, int> Target = Queue.front(); Queue.pop();
		int TargetPosRow = Target.first;
		int TargetPosCol = Target.second;
		if (Visited[TargetPosRow][TargetPosCol]) {
			continue;
		}
		if (NumberTable[TargetPosRow][TargetPosCol] <= 0) {
			continue;
		}
		Visited[TargetPosRow][TargetPosCol] = true;
		world[TargetPosRow][TargetPosCol].itemID = blockID;
		
		if (TargetPosRow + 1 < WORLD_HEIGHT && !Visited[TargetPosRow + 1][TargetPosCol]) {
			int RandomValue = RandomIntRange(1, 100);
			if (RandomValue <= NumberTable[TargetPosRow][TargetPosCol]) {
				NumberTable[TargetPosRow + 1][TargetPosCol] = NumberTable[TargetPosRow][TargetPosCol] - PercentDecreaseAmount;
				Queue.push({ TargetPosRow + 1, TargetPosCol });
			}
		}

		if (TargetPosRow - 1 >= 0 && !Visited[TargetPosRow - 1][TargetPosCol]) {
			int RandomValue = RandomIntRange(1, 100);
			if (RandomValue <= NumberTable[TargetPosRow][TargetPosCol]) {
				NumberTable[TargetPosRow - 1][TargetPosCol] = NumberTable[TargetPosRow][TargetPosCol] - PercentDecreaseAmount;
				Queue.push({ TargetPosRow - 1, TargetPosCol });
			}
		}

		if (TargetPosCol + 1 < WORLD_WIDTH && !Visited[TargetPosRow][TargetPosCol + 1]) {
			int RandomValue = RandomIntRange(1, 100);
			if (RandomValue <= NumberTable[TargetPosRow][TargetPosCol]) {
				NumberTable[TargetPosRow][TargetPosCol + 1] = NumberTable[TargetPosRow][TargetPosCol] - PercentDecreaseAmount;
				Queue.push({ TargetPosRow, TargetPosCol + 1 });
			}
		}

		if (TargetPosCol - 1 >= 0 && !Visited[TargetPosRow][TargetPosCol - 1]) {
			int RandomValue = RandomIntRange(1, 100);
			if (RandomValue <= NumberTable[TargetPosRow][TargetPosCol]) {
				NumberTable[TargetPosRow][TargetPosCol - 1] = NumberTable[TargetPosRow][TargetPosCol] - PercentDecreaseAmount;
				Queue.push({ TargetPosRow, TargetPosCol - 1 });
			}
		}
	}
}

