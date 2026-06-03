#pragma once
#include <string>

class Warship {
public:
	int PosR = 0, PosC = 0;
	int Length = 0;
	int Health = 0;
	std::string ShipName = "";
	
	std::string NameList[6] = {
		"", "", "구축함", "순양함", "전함", "항공모함"
	};

	Warship();

	Warship(int PosR, int PosC, int Length);

	Warship(int PosValue, int Length);

};