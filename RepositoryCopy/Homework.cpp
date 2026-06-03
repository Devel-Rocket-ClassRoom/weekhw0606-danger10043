#include "Homework.h"
#include "Map.h"
#include <iostream>
#include <stdio.h>

void KeyboardWait() {
	printf("[ 계속 진행하려면 아무 키나 입력하세요. ] : ");
	char input; std::cin >> input;
}

void HomeworkRun() {
	srand(time(0));
	Map newMap;
	
	int RemainAttack = 30;
	int RemainBattleShipCount = 4;

	while (RemainAttack > 0 && RemainBattleShipCount > 0) {
		printf("┏━━━━━━━━━━━━━━━━━━━┓\n");
		printf("┃  Battle Ship 게임 ┃ \n");
		printf("┗━━━━━━━━━━━━━━━━━━━┛\n\n");

		newMap.PrintMap(false);
		printf("\n\n 공격할 위치의 좌표(X, Y)를 입력하세요. - 남은 공격 횟수 : [ %d회 ], 적의 남은 함선 : [ %d척 ]\n", RemainAttack, RemainBattleShipCount);
		printf("입력 : ");
		int InputX = 0, InputY = 0;
		std::cin >> InputX >> InputY;

		int AttackFeedback = newMap.Attack(InputY, InputX);
		if (AttackFeedback == -2) {
			printf("<!> - 이미 공격한 위치를 선택하였습니다. 다른 위치를 선택하세요. - <!>\n\n");
			KeyboardWait();
			std::system("cls");
			continue;
		}
		else if (AttackFeedback == -1) {
			printf("<X> - 공격이 적 함선에 명중하지 않았습니다. - <X>\n\n");
			KeyboardWait();
		}
		else {
			printf("<O> - 공격이 적 함선에 명중했습니다! - <O>\n\n");
			if (newMap.WarshipList[AttackFeedback].Health == 0) {
				RemainBattleShipCount--;
				printf("<O> - 적의 %s이 모두 파괴되어 침몰했습니다!! 적의 남은 함선은 [ %d척 ] 입니다. - <O>\n\n", newMap.WarshipList[AttackFeedback].ShipName.c_str(), RemainBattleShipCount);
			}
			KeyboardWait();
		}
		std::system("cls");
		RemainAttack--;
	}

	if (RemainBattleShipCount > 0) {
		printf("┏━━━━━━━━━━━━━━━━━━━┓\n");
		printf("┃  Battle Ship 게임 ┃ \n");
		printf("┗━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("<X> - 상대의 모든 함선을 침몰시키는 것에 실패했습니다. - <X>\n\n");
	}
	else {
		printf("┏━━━━━━━━━━━━━━━━━━━┓\n");
		printf("┃  Battle Ship 게임 ┃ \n");
		printf("┗━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("<O> - 상대의 모든 함선을 침몰시켰습니다! - <O>\n\n");
	}
	printf("[ 상대의 함선 배치도 ]\n");
	newMap.PrintMap(true);
	printf("\n\n[ 당신의 공격 기록 ]\n");
	newMap.PrintMap(false);
	printf("\n\n게임이 종료되었습니다.");

}
