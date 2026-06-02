#include "Map.h"
#include <iostream>
#include <stdio.h>

extern const int MAP_ROW = 10;
extern const int MAP_COLUMN = 10;

Map::Map() {
	// -2 : 플레이어가 이미 공격한 칸.
	// -1 : 배가 없는 칸.
	// 0 ~ 3 : 배가 있는 칸.
	this->Field = new int[MAP_ROW * MAP_COLUMN];
	WarshipList = new Warship[4];

	// 필드의 모든 칸을 배가 없는 칸으로 채우기
	for (int r = 0; r < MAP_ROW; r++) {
		for (int c = 0; c < MAP_COLUMN; c++) {
			*Pos(r, c) = -1;
		}
	}

	// 함선 배치하기
	for (int i = 0; i <= 3; i++) {
		int WarshipPos = PlaceWarship(i + 2, i);
		WarshipList[i] = Warship(WarshipPos, i + 2);
	}
}

int* Map::Pos(int r, int c) {
	return (Field + (MAP_COLUMN) * r + c);
}

int Map::PlaceWarship(int ShipLength, int Code) {
	int TempR = 0;
	int TempC = 0;

	// 적절한 배가 배치될때까지 반복
	while (true) {
		bool hasSuccessd = true;
		int TempR = rand() % MAP_ROW;
		int TempC = rand() % MAP_COLUMN;

		for (int r = 0; r < ShipLength; r++) {
			if (TempR + r >= MAP_ROW) {
				break;
			}
			hasSuccessd = hasSuccessd && (*Pos(TempR + r, TempC) == 0);
		}
		if (hasSuccessd) {
			for (int r = 0; r < ShipLength; r++) {
				*Pos(TempR + r, TempC) = Code;
				return MAP_COLUMN * TempR + TempC;
			}
		}

		hasSuccessd = true;
		for (int c = 0; c < ShipLength; c++) {
			if (TempC + c >= MAP_COLUMN) {
				break;
			}
			hasSuccessd = hasSuccessd && (*Pos(TempR, TempC + c) == 0);
		}
		if (hasSuccessd) {
			for (int c = 0; c < ShipLength; c++) {
				*Pos(TempR, TempC + c) = Code;
				return (MAP_COLUMN * TempR + TempC);
			}
		}
	}
}

void PrintMap() {
	for (int r = 0; r <= MAP_ROW; r++) {
		for (int c = 0; c <= MAP_COLUMN; c++) {
			if (r == 0 && r == c) {
				printf("   ");
			}
			else if (r == 0) {
				printf(" %d ", c - 1);
			}
			else if (r == 0) {
				printf(" %ㅇ ")
			}
		}
	}
}