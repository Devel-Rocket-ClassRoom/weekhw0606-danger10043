#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <windows.h>
#include <random>
#include <map>
#include "itemdata.h"
#include "colorprint.h"
#include "WorldGenerate.h"
#include "random.h"
#include "basic.h"
#include "Player.h"
#include "Item.h"
#include "Block.h"
#include "World.h"
#include "Monster.h"
#include "Battle.h"

int main() {
	srand(time(0));
	Start();

	World newWorld = World();
	WorldGenerator::WorldGenerate(newWorld);
	TextBuffer.push("다음 행동을 입력하세요.");
	TextBuffer.push("행동 목록을 보려면 \"?\" 을 입력하세요.");
	newWorld.printWorld();
	while (true) {
		char command = inputTask();
		switch (command) {
		case 'w':
		case 'a':
		case 's':
		case 'd':
			newWorld.playerMove(command);
			break;
		case 'e':
			newWorld.player.ShowInventory();
			keyboardWait();
			break;
		case 'c':
			newWorld.craft();
			break;
		case 'p':
			newWorld.PlaceBlock();
			break;
		case 'i':
			newWorld.getItemInfo();
			break;
		case 'h':
			newWorld.SwitchItem();
			break;
		case 'u':
			newWorld.useItemInWorld();
			break;
		case 'z':
			newWorld.player.ShowStat();
			break;
		case 't':
			TextBuffer.push("< 개발자 명령어 입니다. >");
			TextBuffer.push("다음 행동 중 하나를 입력하세요.");
			TextBuffer.push("g : 아이템 지급");
			TextBuffer.push("m : 디버깅용 몬스터 소환");
			TextBuffer.push("x : 몬스터 스폰 여부 토글 ");
			TextBuffer.push("d : 플레이어 데미지 입히기");
			TextBuffer.push("b : 플레이어 디버프 적용");
			TextBuffer.push("k : 플레이어 사망");
			TextBuffer.push("t : 시간 조정");
			TextBuffer.push("T : 텔레포트");
			newWorld.printWorld();
			command = inputTask();
			if (command == 'g') {
				newWorld.giveItem();
			}
			else if (command == 'm') {
				TextBuffer.push("소환할 몬스터의 ID를 입력하세요.");
				newWorld.printWorld();
				printf("입력 : ");
				int Input; std::cin >> Input;
				battle(newWorld.player, Input);
			}
			else if (command == 'x') {
				newWorld.player.doesNotMeetMonster = !newWorld.player.doesNotMeetMonster;
				if (newWorld.player.doesNotMeetMonster) {
					TextBuffer.push("이제 몬스터가 스폰되지 않습니다.");
				}
				else {
					TextBuffer.push("이제 몬스터가 다시 스폰됩니다.");
				}
			}
			else if (command == 'd') {
				TextBuffer.push("플레이어에게 입힐 데미지를 정수로 입력하세요.");
				newWorld.printWorld();
				printf("입력 : ");
				int Input; std::cin >> Input;
				TextBuffer.push("플레이어에게 " + std::to_string(newWorld.player.GetDamage(Input, 100)) + "의 피해를 입혔습니다.");
				TextBuffer.push("현재 체력 : [ " + std::to_string(newWorld.player.Health) + " / " + std::to_string(newWorld.player.MaxHealth) + " ]");
			}
			else if (command == 'b') {
				TextBuffer.push("플레이어에게 입힐 디버프 ID와 지속시간을 정수로 입력하세요.");
				newWorld.printWorld();
				printf("입력 : ");
				int DebuffID = 0, Amount = 0; std::cin >> DebuffID >> Amount;
				ApplyDebuff(newWorld.player, (Buff)DebuffID, Amount);
			}
			else if (command == 'k') {
				newWorld.player.clearInventory(newWorld);
			}
			else if (command == 't') {
				TextBuffer.push("월드 시간을 정수로 입력하세요.");
				newWorld.printWorld();
				int Input; std::cin >> Input;
				newWorld.currentTime = Input % 180;
			}
			else if (command == 'T') {
				newWorld.TP();
				break;
			}
			break;
		case '?':
			TextBuffer.push("w 또는 a 또는 s 또는 d : 해당 방향으로 이동합니다.");
			TextBuffer.push("e : 인벤토리를 확인합니다.");
			TextBuffer.push("c : 아이템을 제작합니다.");
			TextBuffer.push("p : 손에 든 블럭을 설치합니다.");
			TextBuffer.push("h : 손에 들 아이템을 설정합니다.");
			TextBuffer.push("u : 손에 든 아이템을 사용합니다.");
			TextBuffer.push("z : 현재 플레이어의 스탯을 확인합니다.");
			TextBuffer.push("i : 아이템의 정보를 확인합니다.");
		}
		newWorld.printWorld();
		//printf("\n현재 시간 : %d\n", newWorld.currentTime);
	}
}