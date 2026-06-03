#include "Map.h"
#include <iostream>
#include <stdio.h>

const int MAP_ROW = 10;
const int MAP_COLUMN = 10;

Map::Map() {
	// -1 : 배가 없는 칸.
	// 0 ~ 3 : 배가 있는 칸. 
	this->Field = new int[MAP_ROW * MAP_COLUMN];
	this->hasAttackedList = new bool[MAP_ROW * MAP_COLUMN];
	this->WarshipList = new Warship[4];

	// 필드의 모든 칸을 배가 없는 칸으로 채우기
	for (int r = 0; r < MAP_ROW; r++) {
		for (int c = 0; c < MAP_COLUMN; c++) {
			*Pos(r, c) = -1;
			*hasAttacked(r, c) = false;
		}
	}

	// 함선 배치하기
	for (int i = 0; i <= 3; i++) {
		int WarshipPos = PlaceWarship(i + 2, i);
		WarshipList[i] = Warship(WarshipPos, i + 2);
	}
}

// 맵의 특정 위치의 정보를 반환
int* Map::Pos(int r, int c) {
	return (Field + (MAP_COLUMN) * r + c);
}

// 맵의 특정 위치에 플레이어가 공격을 시도했는지 여부를 반환
bool* Map::hasAttacked(int r, int c) {
	return (hasAttackedList + (MAP_COLUMN) * r + c);
}

// 맵에 함선을 적절하게 배치하는 함수
int Map::PlaceWarship(int ShipLength, int Code) {
	int TempR = 0;
	int TempC = 0;

	// 적절한 배가 배치될때까지 반복
	while (true) {
		bool hasSuccessd = true;
		TempR = rand() % MAP_ROW;
		TempC = rand() % MAP_COLUMN;

		int Direciton = rand() % 2;

		if (Direciton == 0) {
			for (int r = 0; r < ShipLength; r++) {
				if (TempR + r >= MAP_ROW) {
					hasSuccessd = false;
					break;
				}
				hasSuccessd = hasSuccessd && (*Pos(TempR + r, TempC) == -1);
			}
			if (hasSuccessd) {
				for (int r = 0; r < ShipLength; r++) {
					*Pos(TempR + r, TempC) = Code;
				}
				return MAP_COLUMN * TempR + TempC;
			}
		}

		else {
			hasSuccessd = true;
			for (int c = 0; c < ShipLength; c++) {
				if (TempC + c >= MAP_COLUMN) {
					hasSuccessd = false;
					break;
				}
				hasSuccessd = hasSuccessd && (*Pos(TempR, TempC + c) == -1);
			}
			if (hasSuccessd) {
				for (int c = 0; c < ShipLength; c++) {
					*Pos(TempR, TempC + c) = Code;
				}
				return MAP_COLUMN * TempR + TempC;
			}
		}
	}
}

// 맵을 출력하는 함수
void Map::PrintMap(bool IsGameOver) {
	if (IsGameOver) {
		for (int r = 0; r <= MAP_ROW; r++) {
			for (int c = 0; c <= MAP_COLUMN; c++) {
				if (r == 0 && r == c) {
					printf("   ");
				}
				else if (r == 0) {
					printf("%3d", c - 1);
				}
				else if (c == 0) {
					printf("%3d", r - 1);
				}
				else {
					if (*Pos(r - 1, c - 1) == -1) {
						printf("  .");
					}
					else {
						printf("%3d", *Pos(r - 1, c - 1) + 2);
					}
				}
			}
			printf("\n");
		}
	}
	else {
		for (int r = 0; r <= MAP_ROW; r++) {
			for (int c = 0; c <= MAP_COLUMN; c++) {
				if (r == 0 && r == c) {
					printf("   ");
				}
				else if (r == 0) {
					printf("%3d", c - 1);
				}
				else if (c == 0) {
					printf("%3d", r - 1);
				}
				else {
					if (!(*hasAttacked(r - 1, c - 1))) {
						printf("  .");
					}
					else if (*Pos(r - 1, c - 1) == -1) {
						printf("  X");
					}
					else {
						printf("  O");
					}
				}
			}
			printf("\n");
		}
	}
}

// 맵의 지정한 좌표를 공격하는 함수;
/*
    Result = -2 → 이미 공격한 좌표를 공격 시도함.
	Result = -1 → 배가 없는 곳에 공격 시도함.
	Result = 0 ~ 3 → 배가 있는 곳에 명중함. 명중한 배의 Index 반환.
*/
int Map::Attack(int r, int c) {
	if (*hasAttacked(r, c)) {
		return -2;
	}
	*hasAttacked(r, c) = true;
	int targetValue = *Pos(r, c);
	if (targetValue != -1) {
		WarshipList[targetValue].Health--;
	}

	return targetValue;
}
