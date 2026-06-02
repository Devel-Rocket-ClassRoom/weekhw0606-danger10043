#pragma once
#include "Warship.h"

extern const int MAP_ROW;
extern const int MAP_COLUMN;

class Map {
	int R = 10;
	int C = 10;
	int* Field;
	Warship* WarshipList;

	Map();

	int* Pos(int r, int c);

	int PlaceWarship(int ShipLength, int Code);

	void PrintMap();

};