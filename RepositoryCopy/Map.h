#pragma once
#include "Warship.h"

extern const int MAP_ROW;
extern const int MAP_COLUMN;

class Map {
public:
	int R = 10;
	int C = 10;
	int* Field = nullptr;
	bool* hasAttackedList = nullptr;
	Warship* WarshipList = nullptr;

	Map();

	bool* hasAttacked(int r, int c);

	int* Pos(int r, int c);

	int PlaceWarship(int ShipLength, int Code);

	void PrintMap(bool IsGameOver);

	int Attack(int r, int c);
};