#pragma once
#include "Map.h"

class Warship {
public:
	int PosR = 0, PosC = 0;
	int Length = 0;
	int Health = 0;
	
	Warship() {
		this->PosR = 0;
		this->PosC = 0;
		this->Length = 0;
		this->Health = 0;
	}

	Warship(int PosR, int PosC, int Length) {
		this->PosR = PosR;
		this->PosC = PosC;
		this->Length = Length;
		this->Health = Length;
	}

	Warship(int PosValue, int Length) {
		Warship(PosValue / MAP_COLUMN, PosValue % MAP_COLUMN, Length);
	}

};