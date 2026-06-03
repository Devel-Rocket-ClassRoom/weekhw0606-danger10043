#include "Warship.h"
#include "Map.h"

Warship::Warship() {
	this->PosR = 0;
	this->PosC = 0;
	this->Length = 0;
	this->Health = 0;
	this->ShipName = NameList[0];
}

Warship::Warship(int PosR, int PosC, int Length) {
	this->PosR = PosR;
	this->PosC = PosC;
	this->Length = Length;
	this->Health = Length;
	this->ShipName = NameList[Length];
}

Warship::Warship(int PosValue, int Length) {
	this->PosR = PosValue / MAP_COLUMN;
	this->PosC = PosValue % MAP_COLUMN;
	this->Length = Length;
	this->Health = Length;
	this->ShipName = NameList[Length];
}